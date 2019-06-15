cmake_minimum_required(VERSION 3.12)

set(ENTT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${ENTT_INCLUDE_DIR})