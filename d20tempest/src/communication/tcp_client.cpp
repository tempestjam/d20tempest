#include <nlohmann/json.hpp>

#include "characters/character_factory.hpp"
#include "characters/character.hpp"
#include "communication/tcp_client.hpp"

namespace d20tempest::communication
{   
    class TCPClientImpl
    {
    private:
        character::CharacterFactory m_characterFactory;  
        gsl::not_null<uvw::TCPHandle*> m_clientHandler;

        ClientEventChannel m_iClientEventChannel;

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
                if(m_heloParsed)
                {
                    ParseMsg(std::string(evt.data.get(), evt.length));
                    return;
                }

                uint32_t firstFour = *((uint32_t*)evt.data.get());
                firstFour = ntohl(firstFour);
                if(evt.length >= 4 && firstFour == 0x00C0FFEE)
                {
                    uint32_t ms_reply = 0x000B00B5;
                    ms_reply = htonl(ms_reply);
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
            });

            m_clientHandler->on<uvw::EndEvent>([this](const uvw::EndEvent& evt, uvw::TCPHandle& client)
            {
                m_iClientEventChannel.Emit({IClientEventType::ClientLeave, "", "", nullptr, m_interface});
                client.close();
            });

            m_clientHandler->on<uvw::ErrorEvent>([this](const uvw::ErrorEvent& evt, uvw::TCPHandle& client)
            {
                m_iClientEventChannel.Emit({IClientEventType::ClientError, "", "", nullptr, m_interface});
                client.close();
            });

            m_iClientEventChannel.Emit({IClientEventType::ClientConnect, "", "", nullptr, m_interface});
        }

        ~TCPClientImpl()
        {
            m_clientHandler->close();
        }

        uint64_t ConnectionId() const
        {
            return m_id;
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
            if(docJson.is_discarded() || !docJson.contains("action") || !docJson.contains("entity"))
            {
                nlohmann::json error;
                error["code"] = 400;
                error["msg"] = "JSON malformed";
                Send(error.dump());
                return;
            }
            
            auto action = docJson["action"].get<std::string>();
            auto path = docJson["entity"].get<std::string>();

            std::transform(action.begin(), action.end(), action.begin(), ::toupper); 
            std::transform(path.begin(), path.end(), path.begin(), ::toupper); 

            //Data handler
            m_iClientEventChannel.Emit({IClientEventType::ClientMessage, path, action, docJson["data"], m_interface});
        }

        void GetCharacter()
        {

        }

        void ListCharacters()
        {

        }

        void CreateCharacterHandler(const std::string& name)
        {
            auto character = m_characterFactory.CreateCharacter(name, std::make_optional(m_interface));
            if(character == nullptr)
            {
                nlohmann::json error;
                error["code"] = 409;
                error["msg"] = "Unable to create character (already exist?)";
                Send(error.dump());
                return;
            }

            m_characterFactory.Dump(character->ID());
            Send(character->Save().dump(4));
        }

        void LoadCharacterHandler(const std::string& name)
        {
            auto character = m_characterFactory.LoadCharacter(name, std::make_optional(m_interface));
            if(character == nullptr)
            {
                nlohmann::json error;
                error["code"] = 404;
                error["msg"] = "Unknown entity";
                Send(error.dump());
            }
            Send(character->Save().dump(4));
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