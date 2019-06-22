#include "communication/iclient.hpp" 

#include "characters/character.hpp"
#include "characters/character_factory.hpp"

namespace d20tempest::character
{
    Character::Character(const uint64_t characterID, const std::string& name, std::optional<gsl::not_null<communication::IClient*>> client) : 
            m_characterID(characterID),
            m_name(name),
            m_client(client)
        {
            if(!m_client.has_value())
            {
                return;
            }

            m_client.value()->OnLeave([this]()
            {
                //Save and destroy
                CharacterFactory manager;
                manager.Dump(m_characterID);
            });

            m_client.value()->OnMessage("character/ability" ,[this](const std::string_view& path, 
                                                                    const std::string_view& action, 
                                                                    const nlohmann::json& data, 
                                                                    gsl::not_null<communication::IClient*> client)
            {
                //Change ability
                if(action == "ADD")
                {
                    
                }
                else if(action == "UPDATE")
                {
                    
                }
                else if(action == "GET")
                {
                    
                }
                else if(action == "DELETE")
                {
                    
                }
            });

        }

    std::optional<std::shared_ptr<components::Ability<int>>> Character::AddAbility(const std::string& scriptName, const int defaultValue/* = 0*/)
    {
        if(m_abilities.find(scriptName) != m_abilities.end())
        {
            return {};
        }

        std::stringstream sstream;
        sstream << ms_abilitiesScriptPath << scriptName << ms_scriptExtension; 
        auto ability = std::make_shared<components::Ability<int>>(sstream.str(), defaultValue);
        m_abilities.insert(std::make_pair(scriptName, ability));
        return ability;
    }

    nlohmann::json Character::Save() const
    {
        nlohmann::json root;
        
        root["name"] = m_name;
        root["id"] = m_characterID;

        //Abilities
        std::map<std::string, nlohmann::json> serializedAbilities;
        for(auto&[key, ability] : m_abilities)
        {
            serializedAbilities.insert(std::make_pair(key, ability->Save()));
        }
        root["abilities"] = serializedAbilities;

        return root;
    }

    const void Character::Load(const nlohmann::json& root)
    {
        if(root.is_discarded() &&
            (root.find("name") == root.end() || root.find("abilities") == root.end()) || root.find("id") == root.end())
        {
            throw std::invalid_argument("Content for loading is not in the good format");
        }

        m_characterID = root["id"];
        m_name = root["name"];

        for(auto& [key, value] : root["abilities"].items())
        {
            std::stringstream sstream;
            sstream << ms_abilitiesScriptPath << key << ms_scriptExtension; 
            auto ability = std::make_shared<components::Ability<int>>(sstream.str());
            ability->Load(value);
            m_abilities.insert(std::make_pair(key, ability));
        }
    }

    std::string Character::Name() const
    {
        return m_name;
    }

    uint64_t Character::ID() const
    {
        return m_characterID;
    }
    
} // namespace d20tempest::character
