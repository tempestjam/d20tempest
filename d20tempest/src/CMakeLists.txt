cmake_minimum_required(VERSION 3.12)

set(D20_TEMPEST_INCLUDE ../include)

set(SOURCES
    main.cpp

    communication/tcp_server.cpp
    communication/tcp_client.cpp
    
    party/party_manager.cpp
    party/party.cpp)

set(HEADERS
    ${D20_TEMPEST_INCLUDE}/communication/iclient.hpp
    ${D20_TEMPEST_INCLUDE}/communication/tcp_server.hpp
    ${D20_TEMPEST_INCLUDE}/communication/tcp_client.hpp
    
    ${D20_TEMPEST_INCLUDE}/party/party_manager.hpp
    ${D20_TEMPEST_INCLUDE}/party/party.hpp)

add_executable(d20tempest ${SOURCES} ${HEADERS})
target_include_directories(d20tempest PRIVATE ${D20_TEMPEST_INCLUDE})
target_link_libraries(d20tempest PUBLIC entt gsl uvw uv_a)
target_compile_definitions(d20tempest PRIVATE NOMINMAX _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)