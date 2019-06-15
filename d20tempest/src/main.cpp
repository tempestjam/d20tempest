#include <iostream>

#include "communication/tcp_server.hpp"

int main(int argc, char* argv[])
{
    d20tempest::communication::TCPServer server("127.0.0.1", 8888);
    std::cin.ignore();
}