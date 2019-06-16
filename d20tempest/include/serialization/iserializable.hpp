#pragma once

#include <string_view>

#include <nlohmann/json.hpp>

namespace d20tempest::serialization
{
    class ISerializable
    {
    public:
        virtual nlohmann::json Save() const = 0;
        virtual const void Load(const nlohmann::json& content) = 0;
    };
} // namespace d20tempest::serialization
