#include <iostream>
#include <cassert>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

#include "communication/tcp_server.hpp"
#include "components/ability.hpp"
#include "characters/character.hpp"

int main(int argc, char* argv[])
{
    std::string path = "./scripts/components/abilities";
    for (const auto & entry : fs::directory_iterator(path))
    {
        d20tempest::components::Ability<int> strength(entry.path().generic_string());
        strength.Value(18);
        std::cout << strength.ShortName() << " : " << strength.Modifier() << std::endl;
        strength.Temp(-2);
        std::cout << strength.ShortName() << " : " << strength.Modifier() << std::endl;
        strength.Temp(4);
        std::cout << strength.ShortName() << " : " << strength.Modifier() << std::endl;
        strength.ResetTemp();
        std::cout << strength.ShortName() << " : " << strength.Modifier() << std::endl;
        std::cout << std::endl;
    }

    d20tempest::character::Character charac(0x00, "Aldor");
    charac.AddAbility("strength", 10);
    charac.AddAbility("dexterity", 16);
    charac.AddAbility("constitution", 14);
    charac.AddAbility("intelligence", 18);
    charac.AddAbility("wisdom", 14);
    charac.AddAbility("charism", 20);

    auto val = charac.Save();
    
    d20tempest::character::Character charac2;
    charac2.Load(val);

    std::cout << charac2.Save().dump(4) << std::endl;

    // d20tempest::communication::TCPServer server("0.0.0.0", 7777);
    // std::cout << "Server launched" << std::endl;
    std::cin.ignore();
}