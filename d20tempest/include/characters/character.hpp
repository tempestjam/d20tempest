#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <sstream>
#include <type_traits>

#include "components/ability.hpp"

namespace d20tempest::character
{
    class Character
    {
    private:
        const uint64_t m_characterID;
        const std::string m_name;
        
        std::map<std::string, std::shared_ptr<components::Ability<int>>> m_abilities;
    public:
        static Character Load(const uint64_t characterID)
        {

        }

        static Character Create(const std::string& name)
        {

        }

        Character(const uint64_t characterID, const std::string& name) : 
            m_characterID(characterID),
            m_name(name)
        {

        }

        Character(const Character& other) = default;
        Character(Character&& other) = default;

        virtual ~Character() = default;

        Character& operator=(const Character& rhs) = default;
        Character& operator=(Character&& rhs) = default;

        std::optional<std::shared_ptr<components::Ability<int>>> AddAbility(const std::string& scriptName, const int defaultValue = 0)
        {
            if(m_abilities.find(scriptName) != m_abilities.end())
            {
                return {};
            }

            std::stringstream sstream;
            sstream << "./scripts/components/abilities/" << scriptName << ".lua"; 
            auto ability = std::make_shared<components::Ability<int>>(sstream.str(), defaultValue);
            m_abilities.insert(std::make_pair(ability->ShortName(), ability));
            return ability;
        }
    };
}