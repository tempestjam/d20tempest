#include "communication/tcp_client.hpp"

namespace d20tempest::communication
{   
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
                std::string_view dataView(evt.data.get(), evt.length);

                for(auto& h : m_messageHandlers)
                {
                    h(dataView);
                }
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

        virtual void OnMessage(ClientMessageHandler handler)
        {
            if(handler == nullptr)
            {
                return;
            }

            m_messageHandlers.push_back(handler);
        }

        virtual void OnLeave(ClientDisconnectedHandler handler)
        {
            if(handler == nullptr)
            {
                return;
            }

            m_disconnectionHandlers.push_back(handler);
        }

        virtual void Send(std::vector<std::byte>& msg)
        {
            if(!m_clientHandler->active())
            {
                return;
            }

            m_clientHandler->write(reinterpret_cast<char*>(msg.data()), msg.size());
        }

        virtual void Send(std::vector<std::byte>&& msg)
        {
            if(!m_clientHandler->active())
            {
                return;
            }

            m_clientHandler->write(reinterpret_cast<char*>(msg.data()), msg.size());
            msg.clear();
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