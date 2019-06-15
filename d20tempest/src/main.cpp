#include <iostream>
#include <cassert>

#include "communication/tcp_server.hpp"

#include <sol3/sol.hpp>

int main(int argc, char* argv[])
{
    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x]{ ++x; });
    lua.script("beep()");
    assert(x == 1);

    d20tempest::communication::TCPServer server("127.0.0.1", 8888);
    std::cout << "Server launched" << std::endl;
    std::cin.ignore();
}