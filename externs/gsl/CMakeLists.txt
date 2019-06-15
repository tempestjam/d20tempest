cmake_minimum_required(VERSION 3.12)

set(GSL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(gsl INTERFACE)
target_include_directories(gsl INTERFACE ${GSL_INCLUDE_DIR})