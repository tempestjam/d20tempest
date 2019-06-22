#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <sstream>
#include <type_traits>

#include <gsl/gsl>

#include "components/ability.hpp"
#include "communication/iclient.hpp" 

#include "serialization/iserializable.hpp" 

namespace d20tempest::character
{
    class Character : public serialization::ISerializable
    {
    private:
        uint64_t m_characterID;
        std::string m_name;
        
        std::optional<gsl::not_null<communication::IClient*>> m_client;

        communication::ClientEventChannel m_clientEventChannel;

        static constexpr const char ms_charactersPath[] = "./characters/";

        static constexpr char ms_abilitiesScriptPath[] = "./scripts/components/abilities/";
        static constexpr char ms_scriptExtension[] = ".lua";

        std::map<std::string, std::shared_ptr<components::Ability<int>>> m_abilities;
    public:
        Character() = default;
        Character(const uint64_t characterID, const std::string& name, std::optional<gsl::not_null<communication::IClient*>> client);

        Character(const Character& other) = default;
        Character(Character&& other) = default;

        virtual ~Character() = default;

        Character& operator=(const Character& rhs) = default;
        Character& operator=(Character&& rhs) = default;

        std::string Name() const;
        uint64_t ID() const;

        void Save();

        std::optional<std::shared_ptr<components::Ability<int>>> AddAbility(const std::string& scriptName, const int defaultValue = 0);

        virtual nlohmann::json Serialize() const override;
        virtual const void Deserialize(const nlohmann::json& content) override;
    };
}