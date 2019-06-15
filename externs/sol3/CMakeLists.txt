cmake_minimum_required(VERSION 3.12)

set(SOL3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(sol3 INTERFACE)
target_include_directories(sol3 INTERFACE ${SOL3_INCLUDE_DIR})