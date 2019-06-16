#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <sstream>
#include <type_traits>

#include "components/ability.hpp"

#include "serialization/iserializable.hpp" 

namespace d20tempest::character
{
    class Character : public serialization::ISerializable
    {
    private:
        uint64_t m_characterID;
        std::string m_name;
        
        static constexpr char ms_abilitiesScriptPath[] = "./scripts/components/abilities/";
        static constexpr char ms_scriptExtension[] = ".lua";

        std::map<std::string, std::shared_ptr<components::Ability<int>>> m_abilities;
    public:
        Character() = default;
        Character(const uint64_t characterID, const std::string& name) : 
            m_characterID(characterID),
            m_name(name) {}

        Character(const Character& other) = default;
        Character(Character&& other) = default;

        virtual ~Character() = default;

        Character& operator=(const Character& rhs) = default;
        Character& operator=(Character&& rhs) = default;

        std::optional<std::shared_ptr<components::Ability<int>>> AddAbility(const std::string& scriptName, const int defaultValue = 0);

        virtual nlohmann::json Save() const;
        virtual const void Load(const nlohmann::json& content);
    };
}