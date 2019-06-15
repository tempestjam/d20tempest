cmake_minimum_required(VERSION 3.12)

project(d20tempest 
        VERSION 0.0.0
        DESCRIPTION "A D20 RPG manager"
        LANGUAGES C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/output/bin)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(externs)
add_subdirectory(d20tempest/src)