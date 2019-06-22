#pragma once

#include <functional>
#include <optional>

#include <gsl/gsl>

#include <nlohmann/json.hpp>

#include "event/event_channel.hpp"

namespace d20tempest::communication
{
    class IClient;

    enum class IClientEventType : uint32_t
    {
        ClientLeave,
        ClientError,
        ClientConnect,
        ClientMessage
    };

    enum class IClientAction : uint32_t
    {
        None,
        Get,
        Create,
        Add,
        Update
    };

    struct IClientEventArg
    {
        IClientEventType EventType;
        const std::string_view& Entity;
        const IClientAction Action;
        const nlohmann::json& Data;
        gsl::not_null<IClient*> Client;
    };

    using ClientEventChannel = event::EventChannel<event::Events::IClientEvent, IClientEventArg>;

    class IClient
    {
    public:
        virtual void Send(std::vector<std::byte>& msg) = 0;
        virtual void Send(std::vector<std::byte>&& msg) = 0;
    };
}