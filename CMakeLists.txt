cmake_minimum_required(VERSION 3.16)
project(locket_api)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(api)
add_subdirectory(emulator)
add_subdirectory(examples)

add_executable(locket_api main.cpp)
target_link_libraries(locket_api api)

