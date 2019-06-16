cmake_minimum_required(VERSION 3.12)

set(JSONCPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE ${JSONCPP_INCLUDE_DIR})