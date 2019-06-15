#include <nlohmann/json.hpp>

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

        std::vector<ClientMessageHandler> m_messageHandlers;
        std::vector<ClientDisconnectedHandler> m_disconnectionHandlers;

        const uint64_t m_id;
    public:
        TCPClientImpl(gsl::not_null<uvw::TCPHandle*> sock, const uint64_t connectionID) : 
            m_clientHandler(sock),
            m_id(connectionID)
        {
            m_clientHandler->on<uvw::DataEvent>([this](const uvw::DataEvent& evt, uvw::TCPHandle& client)
            {
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

        void OnMessage(ClientMessageHandler handler)
        {
            if(handler == nullptr)
            {
                return;
            }

            m_messageHandlers.push_back(handler);
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
                //Send back error
                return;
            }
            
            //A msg should at least contains a msg or a partyID
            if(!docJson.contains("msg"))
            {
                //Send back error
                return;
            }

            switch (docJson["msg"].get<uint64_t>())
            {
            case static_cast<uint64_t>(Messages::ConnectionMsg):
                //Create the player

                break;
            case static_cast<uint64_t>(Messages::JoinPartyMsg):
                //Send join party event

                break;
            case static_cast<uint64_t>(Messages::LeavePartyMsg):
                //Send leave party event
                
                break;
            default:
                //Other message
                for(auto& h : m_messageHandlers)
                {
                    h(dataView);
                }
                break;
            }
        }
    };

    TCPClient::TCPClient(gsl::not_null<uvw::TCPHandle*> sock, const uint64_t connectionID)
    {
        m_impl = std::make_shared<TCPClientImpl>(sock, connectionID);
    }
    
    TCPClient::~TCPClient()
    {
        m_impl = nullptr;
    }

    void TCPClient::OnMessage(ClientMessageHandler handler)
    {
        m_impl->OnMessage(handler);
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