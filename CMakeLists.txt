cmake_minimum_required(VERSION 3.16)
project(locket_api LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_subdirectory(embedded)
else()
    add_subdirectory(emulator)
endif()

add_subdirectory(api)
add_subdirectory(examples)
