cmake_minimum_required(VERSION 3.12)

set(SOURCES
    main.cpp)

add_executable(d20tempest ${SOURCES})
target_include_directories(d20tempest PRIVATE ../include)
target_link_libraries(d20tempest gsl entt)