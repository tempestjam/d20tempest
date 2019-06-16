#include <nlohmann/json.hpp>

#include "characters/character_manager.hpp"
#include "communication/tcp_client.hpp"

namespace d20tempest::communication
{   
    enum class Messages : uint64_t
    {
        LeavePartyMsg = 0x00,
        JoinPartyMsg  = 0x01,
        ConnectionMsg = 0x02,
    };

    class TCPClientImpl
    {
    private:
        gsl::not_null<uvw::TCPHandle*> m_clientHandler;

        std::map<std::string, std::vector<ClientMessageHandler>> m_messageHandlers;
        std::vector<ClientDisconnectedHandler> m_disconnectionHandlers;

        bool m_heloParsed = false;

        gsl::not_null<IClient*> m_interface;

        const uint64_t m_id;
    public:
        TCPClientImpl(gsl::not_null<uvw::TCPHandle*> sock, gsl::not_null<IClient*> codeInterface, const uint64_t connectionID) : 
            m_clientHandler(sock),
            m_id(connectionID),
            m_interface(codeInterface)
        {
            m_clientHandler->on<uvw::DataEvent>([this](const uvw::DataEvent& evt, uvw::TCPHandle& client)
            {
                if(*((uint32_t*)evt.data.get()) == 0x00C0FFEE)
                {
                    uint32_t ms_reply = 0x000B00B5;
                    Send(std::vector<std::byte>(reinterpret_cast<std::byte*>(&ms_reply), reinterpret_cast<std::byte*>(&ms_reply) + 4));
                    m_heloParsed = true;
                    return;
                }
                
                if(!m_heloParsed)
                {
                    nlohmann::json error;
                    error["code"] = 400;
                    error["msg"] = "HELO was not received";
                    Send(error.dump());
                    return;
                }

                ParseMsg(std::string(evt.data.get(), evt.length));
            });

            m_clientHandler->on<uvw::EndEvent>([this](const uvw::EndEvent& evt, uvw::TCPHandle& client)
            {
                for(auto& h : m_disconnectionHandlers)
                {
                    h();
                }
            });

            m_clientHandler->on<uvw::ErrorEvent>([this](const uvw::ErrorEvent& evt, uvw::TCPHandle& client)
            {
                
            });
        }

        ~TCPClientImpl()
        {
            m_clientHandler->close();
        }

        uint64_t ConnectionId() const
        {
            return m_id;
        }

        void OnMessage(const std::string& path, ClientMessageHandler handler)
        {
            if(handler == nullptr)
            {
                return;
            }

            std::string toUpperPath = "";
            std::transform(path.begin(), path.end(), toUpperPath.begin(), ::toupper); 

            if(m_messageHandlers.find(toUpperPath) == m_messageHandlers.end())
            {
                m_messageHandlers.insert(std::make_pair(toUpperPath, std::vector<ClientMessageHandler>()));
            }

            m_messageHandlers.at(toUpperPath).push_back(handler);
        }

        void OnLeave(ClientDisconnectedHandler handler)
        {
            if(handler == nullptr)
            {
                return;
            }

            m_disconnectionHandlers.push_back(handler);
        }

        void Send(std::vector<std::byte>& msg)
        {
            if(!m_clientHandler->active())
            {
                return;
            }

            m_clientHandler->write(reinterpret_cast<char*>(msg.data()), gsl::narrow<unsigned int, size_t>(msg.size()));
        }

        void Send(std::vector<std::byte>&& msg)
        {
            if(!m_clientHandler->active())
            {
                return;
            }

            m_clientHandler->write(reinterpret_cast<char*>(msg.data()), gsl::narrow<unsigned int, size_t>(msg.size()));
            msg.clear();
        }

        void Send(std::string& msg)
        {
             if(!m_clientHandler->active())
            {
                return;
            }

            m_clientHandler->write(msg.data(), gsl::narrow<unsigned int, size_t>(msg.size()));
        }

    private:
        void ParseMsg(const std::string& dataView)
        {
            auto docJson = nlohmann::json::parse(dataView, nullptr, false);
            if(docJson.is_discarded())
            {
                nlohmann::json error;
                error["code"] = 400;
                error["msg"] = "JSON malformed";
                Send(error.dump());
                return;
            }
            
            //A msg should at least contains a msg or a partyID
            if(!docJson.contains("name") || !docJson.contains("action") || !docJson.contains("data") || !docJson.contains("entity"))
            {
                nlohmann::json error;
                error["code"] = 400;
                error["msg"] = "Missing elements in JSON";
                Send(error.dump());
                return;
            }

            auto action = docJson["action"].get<std::string>();
            auto path = docJson["entity"].get<std::string>();
            auto name = docJson["name"].get<std::string>();

            std::transform(action.begin(), action.end(), action.begin(), ::toupper); 
            std::transform(path.begin(), path.end(), path.begin(), ::toupper); 

            //Create character handler
            if(action == "CREATE" && path == "CHARACTER")
            {
                CreateCharacterHandler(name, docJson["data"]);
                return;
            }

            //Load character handler
            if(action == "LOAD" & path == "CHARACTER")
            {
                LoadCharacterHandler(name, docJson["data"]);
                return;
            }

            //Data handler
            DataCharacterHandler(name, path, action, docJson["data"]);
        }

        void CreateCharacterHandler(const std::string& name, const nlohmann::json& docJson)
        {
            auto character = character::CharacterManager::CreateCharacter(name, std::make_optional(m_interface));
            if(character == nullptr)
            {
                nlohmann::json error;
                error["code"] = 409;
                error["msg"] = "Unable to create character (already exist?)";
                Send(error.dump());
            }
        }

        void LoadCharacterHandler(const std::string& name, const nlohmann::json& docJson)
        {
            auto character = character::CharacterManager::LoadCharacter(name, std::make_optional(m_interface));
            if(character == nullptr)
            {
                nlohmann::json error;
                error["code"] = 404;
                error["msg"] = "Unknown entity";
                Send(error.dump());
            }
        }

        void DataCharacterHandler(const std::string& name, const std::string& entity, const std::string& action, const nlohmann::json& data)
        {
            if(m_messageHandlers.find(entity) == m_messageHandlers.end())
            {
                nlohmann::json error;
                error["code"] = 404;
                error["msg"] = "Unknown entity";
                Send(error.dump());
                return;
            }

            for(auto& h : m_messageHandlers.at(entity))
            {
                h(entity, action, data);
            }
        }
    };

    TCPClient::TCPClient(gsl::not_null<uvw::TCPHandle*> sock, const uint64_t connectionID)
    {
        m_impl = std::make_shared<TCPClientImpl>(sock, gsl::make_not_null<TCPClient*>(this), connectionID);
    }
    
    TCPClient::~TCPClient()
    {
        m_impl = nullptr;
    }

    void TCPClient::OnMessage(const std::string& path, ClientMessageHandler handler)
    {
        m_impl->OnMessage(path, handler);
    }

    void TCPClient::OnLeave(ClientDisconnectedHandler handler)
    {
        m_impl->OnLeave(handler);
    }

    void TCPClient::Send(std::vector<std::byte>& msg)
    {
        return m_impl->Send(msg);
    }

    void TCPClient::Send(std::vector<std::byte>&& msg)
    {
        return m_impl->Send(msg);
    }

    uint64_t TCPClient::ConnectionId() const
    {
        return m_impl->ConnectionId();
    }
}