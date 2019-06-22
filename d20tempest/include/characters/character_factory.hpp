#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>

#include <gsl/gsl>

#include <nlohmann/json.hpp>

#include "communication/iclient.hpp"

namespace d20tempest::character
{
    class Character;
    class CharacterFactory
    {
        friend class Character;
    private:
        static std::map<uint64_t, std::shared_ptr<Character>> ms_characters;
    public:
        std::shared_ptr<Character> CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);
        std::shared_ptr<Character> LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);

        // void Dump();
        // void Dump(uint64_t id);
        // nlohmann::json DumpAll();
        // nlohmann::json ListExistingCharacters();

        uint64_t CreateCharacterID();

        // std::optional<std::shared_ptr<Character>> operator[](uint64_t id);
    };
}