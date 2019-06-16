#include <atomic>
#include <map>
#include <thread>

#include <uvw.hpp>

#include "communication/tcp_server.hpp"
#include "communication/tcp_client.hpp"

namespace d20tempest::communication
{
    class TCPServerImpl
    {
    private:
        std::shared_ptr<uvw::TCPHandle> m_serverHandle;
        std::unique_ptr<std::thread> m_runningThread;

        std::map<uint64_t, std::shared_ptr<TCPClient>> m_clients;
    public:
        TCPServerImpl(const std::string& iface, const uint16_t port)
        {
            static std::atomic_uint64_t ms_currentClientID = 0;
            m_serverHandle = uvw::Loop::getDefault()->resource<uvw::TCPHandle>();

            m_serverHandle->on<uvw::ListenEvent>([this](const uvw::ListenEvent &, uvw::TCPHandle &srv)
            {
                auto clientHandle = srv.loop().resource<uvw::TCPHandle>();
                clientHandle->on<uvw::CloseEvent>([this](const uvw::CloseEvent& evt, uvw::TCPHandle& handle) 
                { 
                    auto client = std::static_pointer_cast<TCPClient>(handle.data()); 
                    if(auto it = m_clients.find(client->ConnectionId()); it != m_clients.end())
                    {
                        m_clients.erase(it);
                    }    
                });

                srv.accept(*clientHandle);

                auto client = std::make_shared<TCPClient>(gsl::make_not_null(clientHandle.get()), ms_currentClientID++);
                clientHandle->data(client);
                m_clients.insert(std::make_pair(client->ConnectionId(), client));

                clientHandle->read();
            });

            m_serverHandle->bind(iface, port);
            m_serverHandle->listen();
            m_runningThread = std::make_unique<std::thread>([this]()
            {
                uvw::Loop::getDefault()->run();
            });
        }

        ~TCPServerImpl()
        {
            m_serverHandle->close();
            uvw::Loop::getDefault()->stop();
            if(m_runningThread != nullptr && m_runningThread->joinable())
            {
                m_runningThread->join();
            }
        }
    };
    
    TCPServer::TCPServer(const std::string& iface, const uint16_t port)
    {
        m_impl = std::make_shared<TCPServerImpl>(iface, port);
    }
    
    TCPServer::~TCPServer()
    {
        m_impl = nullptr;
    }
} // namespace name
