#pragma once

#include <functional>
#include <optional>

#include <gsl/gsl>

#include <nlohmann/json.hpp>

namespace d20tempest::communication
{
    class IClient;
    using ClientMessageHandler = std::function<void(const std::string_view& path, const std::string_view& method, const nlohmann::json& data, gsl::not_null<IClient*> client)>;
    using ClientDisconnectedHandler = std::function<void()>;

    class IClient
    {
    public:
        virtual void OnMessage(const std::string& path, ClientMessageHandler handler) = 0;
        virtual void OnLeave(ClientDisconnectedHandler handler) = 0;

        virtual void Send(std::vector<std::byte>& msg) = 0;
        virtual void Send(std::vector<std::byte>&& msg) = 0;
    };
}