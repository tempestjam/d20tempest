#include "characters/character_factory.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <sstream>

#include "characters/character.hpp"

namespace d20tempest::character
{
    CharacterFactory::CharacterFactory()
    {
        m_eventKey = m_clientEventChannel.Register([this](const communication::IClientEventArg& arg)
        {
            if(arg.EventType != communication::IClientEventType::ClientMessage
                || arg.Entity != CharacterFactoryEntity
                || (arg.Action != communication::IClientAction::Create && arg.Action != communication::IClientAction::Get))
            {
                return;
            }

            if(arg.Action == communication::IClientAction::Create && !arg.Data.is_null())
            {
                //Create character
                
            }
            else if(arg.Action == communication::IClientAction::Get && !arg.Data.is_null())
            {
                //Load a character
            }
            else if(arg.Action == communication::IClientAction::Get && arg.Data.is_null())
            {
                //Get existing characters
            }
        });
    }

    CharacterFactory::~CharacterFactory()
    {
        m_clientEventChannel.Unregister(m_eventKey);
    }

    std::optional<Character> CharacterFactory::CreateCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << Character::ms_charactersPath << name;

        if(fs::exists(sstream.str()))
        {
            return {};
        }

        Character character(CreateCharacterID(), name, client);
        return character;
    }

    std::optional<Character> CharacterFactory::LoadCharacter(std::string name, std::optional<gsl::not_null<communication::IClient*>> client)
    {
        std::stringstream sstream;
        sstream << Character::ms_charactersPath << name;

        if(!fs::exists(sstream.str()))
        {
            return {};
        }

        std::ifstream file(sstream.str());
        nlohmann::json characterData;
        file >> characterData;

        Character character(CreateCharacterID(), name, client);
        character.Deserialize(characterData);

        file.close();
        return character;
    }

    uint64_t CharacterFactory::CreateCharacterID()
    {
        return 0x00ui64;
    }
} // namespace d20tempest::character
