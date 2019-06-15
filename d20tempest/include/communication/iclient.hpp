#pragma once

#include <functional>
#include <optional>

namespace d20tempest::communication
{
    using ClientMessageHandler = std::function<void(const std::string_view& msg)>;
    using ClientDisconnectedHandler = std::function<void()>;

    class IClient
    {
    public:
        virtual void OnMessage(ClientMessageHandler handler) = 0;
        virtual void OnLeave(ClientDisconnectedHandler handler) = 0;

        virtual void Send(std::vector<std::byte>& msg) = 0;
        virtual void Send(std::vector<std::byte>&& msg) = 0;
    };
}