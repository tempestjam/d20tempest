#include <iostream>
#include <string>

#include "communication/tcp_server.hpp"
#include "characters/character_factory.hpp"

int main(int argc, char* argv[])
{
    // auto character = d20tempest::character::CharacterManager::LoadCharacter("Aldor", {});
    
    // character->AddAbility("strength", 10);
    // character->AddAbility("dexterity", 16);
    // character->AddAbility("constitution", 14);
    // character->AddAbility("intelligence", 18);
    // character->AddAbility("wisdom", 14);
    // character->AddAbility("charism", 20);
   
    d20tempest::character::CharacterFactory factory;
    d20tempest::communication::TCPServer server("0.0.0.0", 7777);
    std::cout << "Server launched" << std::endl;

    std::cin.ignore();
}