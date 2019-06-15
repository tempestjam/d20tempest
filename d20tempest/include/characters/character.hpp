#pragma once

#include <entt/entt.hpp>

namespace d20tempest::character
{
    class Character
    {
    private:
        const entt::entity m_entity;
        const uint64_t m_characterID;

    public:
        static Character Load(const uint64_t characterID);
        static Character Create(const std::string& name);

        Character(const entt::entity entityID, const uint64_t characterID);
        Character(const Character& other);
        Character(const Character&& other);
        ~Character();

        Character& operator=(const Character& rhs);
        Character& operator=(Character&& rhs);

        //Conversion into entity
        operator entt::entity();
    };
}