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


if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)

    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
            EXCLUDE_FROM_ALL)

    include(GoogleTest)

    enable_testing()

    # Now simply link against gtest or gtest_main as needed. Eg
    add_executable(example example.cpp)
    target_link_libraries(example gtest_main)
    gtest_discover_tests(example)

    # Now simply link against gtest or gtest_main as needed. Eg
    add_executable(example2 example2.cpp)
    target_link_libraries(example2 gtest_main)
    gtest_discover_tests(example2)

endif()
