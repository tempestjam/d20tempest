#pragma once

#include <functional>
#include <memory>
#include <string>

namespace d20tempest::communication
{
    using ConnectionHandler = std::function<void()>;
    using DisconnectionHandler = std::function<void()>;

    class TCPServerImpl;
    class TCPServer
    {
    private:
        std::shared_ptr<TCPServerImpl> m_impl;
    public:
        TCPServer(const std::string& iface, const uint16_t port);
        ~TCPServer();
    };
} // namespace d20tempest::communication
