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
    public:
        CharacterFactory();

        CharacterFactory(const CharacterFactory& other) = delete;
        CharacterFactory(CharacterFactory&& other) = delete;

        CharacterFactory& operator=(const CharacterFactory& rhs) = delete;
        CharacterFactory& operator=(CharacterFactory&& rhs) = delete;

        ~CharacterFactory();

        std::optional<Character> CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);
        std::optional<Character> LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client);

    private:
        uint64_t CreateCharacterID();
        communication::ClientEventChannel m_clientEventChannel;
        event::EventKey m_eventKey;

        static constexpr char CharacterFactoryEntity[] = "CHARACTERS";
    };
}