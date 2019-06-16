#include <iostream>
#include <cassert>
#include <chrono>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

#include "communication/tcp_server.hpp"
#include "components/ability.hpp"
#include "characters/character_manager.hpp"

int main(int argc, char* argv[])
{
    // auto character = d20tempest::character::CharacterManager::LoadCharacter("Aldor", {});
    
    // character->AddAbility("strength", 10);
    // character->AddAbility("dexterity", 16);
    // character->AddAbility("constitution", 14);
    // character->AddAbility("intelligence", 18);
    // character->AddAbility("wisdom", 14);
    // character->AddAbility("charism", 20);
   
    d20tempest::communication::TCPServer server("0.0.0.0", 7777);
    std::cout << "Server launched" << std::endl;

    std::cin.ignore();

    d20tempest::character::CharacterManager::Dump();
}