#include <iostream>
#include <cassert>
#include <string>

#include "communication/tcp_server.hpp"
#include "components/ability.hpp"

int main(int argc, char* argv[])
{
    d20tempest::components::Ability<int> strength("./scripts/components/abilities/strength.lua");
    strength.Value(18);
    std::cout << strength.Modifier() << std::endl;
    strength.Temp(-2);
    std::cout << strength.Modifier() << std::endl;
    strength.Temp(4);
    std::cout << strength.Modifier() << std::endl;
    strength.ResetTemp();
    std::cout << strength.Modifier() << std::endl;

    // d20tempest::communication::TCPServer server("0.0.0.0", 7777);
    // std::cout << "Server launched" << std::endl;
    std::cin.ignore();
}