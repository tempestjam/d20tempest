#include "characters/character_manager.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <sstream>

namespace d20tempest::character
{
    std::map<uint64_t, std::shared_ptr<Character>> CharacterManager::m_characters;

    std::shared_ptr<Character> CharacterManager::CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << ms_charactersPath << name;

        if(fs::exists(sstream.str()))
        {
            return nullptr;
        }

        auto character = std::make_shared<Character>(CreateCharacterID(), name, client);
        m_characters.insert(std::make_pair(character->ID(), character));
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

        for(auto& [key, value] : m_characters)
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
} // namespace d20tempest::character
