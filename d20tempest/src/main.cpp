#include <iostream>
#include <cassert>
#include <string>

#include "communication/tcp_server.hpp"

int main(int argc, char* argv[])
{
    d20tempest::communication::TCPServer server("0.0.0.0", 7777);
    std::cout << "Server launched" << std::endl;
    std::cin.ignore();
}