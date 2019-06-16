#include "characters/character_manager.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <sstream>

#include "characters/character.hpp"

namespace d20tempest::character
{
    std::map<uint64_t, std::shared_ptr<Character>> CharacterManager::ms_characters;

    std::shared_ptr<Character> CharacterManager::CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << ms_charactersPath << name;

        if(fs::exists(sstream.str()))
        {
            return nullptr;
        }

        auto character = std::make_shared<Character>(CreateCharacterID(), name, client);
        ms_characters.insert(std::make_pair(character->ID(), character));
        return character;
    }

    std::shared_ptr<Character> CharacterManager::LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << ms_charactersPath << name;

        if(!fs::exists(sstream.str()))
        {
            return nullptr;
        }

        std::ifstream file(sstream.str());
        nlohmann::json characterData;
        file >> characterData;

        auto character = std::make_shared<Character>(CreateCharacterID(), name, client);
        character->Load(characterData);

        file.close();
        return character;
    }

    uint64_t CharacterManager::CreateCharacterID()
    {
        return 0x00ui64;
    }

    void CharacterManager::Dump()
    {
        if(!fs::exists(ms_charactersPath))
        {
            fs::create_directories(ms_charactersPath);
        }

        for(auto& [key, value] : ms_characters)
        {
            std::stringstream sstream;
            sstream << ms_charactersPath << value->Name();

            if(fs::exists(sstream.str()))
            {
                fs::remove(sstream.str());
            }
            std::ofstream file(sstream.str(), std::ios::binary);
            file << std::setw(4) << value->Save();
            file.flush();
            file.close();
        }
    }

    void CharacterManager::Dump(uint64_t id)
    {
        if(ms_characters.find(id) == ms_characters.end())
        {
            return;
        }

        if(!fs::exists(ms_charactersPath))
        {
            fs::create_directories(ms_charactersPath);
        }

        auto character = ms_characters[id];
        std::stringstream sstream;
        sstream << ms_charactersPath << character->Name();

        if(fs::exists(sstream.str()))
        {
            fs::remove(sstream.str());
        }
        std::ofstream file(sstream.str(), std::ios::binary);
        file << std::setw(4) << character->Save();
        file.flush();
        file.close();
    }

    nlohmann::json CharacterManager::DumpAll()
    {
        nlohmann::json dat;

        for(auto& [key, value] : ms_characters)
        {
            dat[key] = value->Save();
        }

        return dat;
    }

    nlohmann::json CharacterManager::ListExistingCharacters()
    {
        nlohmann::json dat;
        //TODO : implement
        return dat;
    }

    std::optional<std::shared_ptr<Character>> CharacterManager::operator[](uint64_t id)
    {
        if(ms_characters.find(id) == ms_characters.end())
        {
            return {};
        }

        return ms_characters.at(id);
    }
} // namespace d20tempest::character
