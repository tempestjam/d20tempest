#
# uvw
#

cmake_minimum_required(VERSION 3.2)

#
# Building in-tree is not allowed (we take care of your craziness).
#

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

#
# Project configuration
#

project(uvw VERSION 1.17.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(PROJECT_AUTHOR "Michele Caini")
set(PROJECT_AUTHOR_EMAIL "michele.caini@gmail.com")

message("*")
message("* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message("* Copyright (c) 2016-2019 ${PROJECT_AUTHOR} <${PROJECT_AUTHOR_EMAIL}>")
message("*")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -DRELEASE")
endif()

#
# CMake configuration
#

set(PROJECT_CMAKE_IN ${uvw_SOURCE_DIR}/cmake/in)
set(PROJECT_DEPS_DIR ${uvw_SOURCE_DIR}/deps)
set(PROJECT_SRC_DIR ${uvw_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

#
# Referenced packages
#

set(THREADS_PREFER_PTHREAD_FLAG ON)

include(FindThreads)

add_library(uvw INTERFACE)
target_include_directories(uvw INTERFACE ${PROJECT_SRC_DIR})