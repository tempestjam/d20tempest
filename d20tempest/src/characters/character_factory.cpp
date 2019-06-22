#include "characters/character_factory.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <sstream>

#include "characters/character.hpp"

namespace d20tempest::character
{
    std::map<uint64_t, std::shared_ptr<Character>> CharacterFactory::ms_characters;

    std::shared_ptr<Character> CharacterFactory::CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << Character::ms_charactersPath << name;

        if(fs::exists(sstream.str()))
        {
            return nullptr;
        }

        auto character = std::make_shared<Character>(CreateCharacterID(), name, client);
        ms_characters.insert(std::make_pair(character->ID(), character));
        return character;
    }

    std::shared_ptr<Character> CharacterFactory::LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << Character::ms_charactersPath << name;

        if(!fs::exists(sstream.str()))
        {
            return nullptr;
        }

        std::ifstream file(sstream.str());
        nlohmann::json characterData;
        file >> characterData;

        auto character = std::make_shared<Character>(CreateCharacterID(), name, client);
        character->Deserialize(characterData);

        file.close();
        return character;
    }

    uint64_t CharacterFactory::CreateCharacterID()
    {
        return 0x00ui64;
    }
} // namespace d20tempest::character
