#include <iostream>

#include <entt/entt.hpp>
#include <gsl/gsl>

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

int main(int argc, char* argv[])
{
    //Test entt
    entt::registry registry;
    for(auto i = 0; i < 10; ++i) 
    {
        auto entity = registry.create();
        registry.assign<position>(entity, i * 1.f, i * 1.f);
        if(i % 2 == 0) { registry.assign<velocity>(entity, i * .1f, i * .1f); }
    }

    //Test GSL and C++17
    std::byte foo = gsl::narrow_cast<std::byte>(0x00);
    auto notNull = gsl::make_not_null<std::byte*>(&foo);

    std::cout << "Hello world" << std::endl;
    std::cin.ignore();
}