#pragma once

#include <map>
#include <memory>
#include <string>

#include "characters/character_manager.hpp"
#include "characters/character.hpp"

namespace d20tempest::character
{
    class CharacterManager
    {
    private:
        static std::map<uint64_t, std::shared_ptr<Character>> m_characters;

        static constexpr const char ms_charactersPath[] = "./characters/";
    public:
        static std::shared_ptr<Character> CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);
        static std::shared_ptr<Character> LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);

        static void Dump();

        static uint64_t CreateCharacterID();
    };
}