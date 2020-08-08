cmake_minimum_required(VERSION 3.16)
project(locket_api LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# If compiling in firmware mode - set bunch of hardware-specific compiler flags
if(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m3 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -g -fstack-usage -Wall -fno-threadsafe-statics -MMD -MP -specs=nano.specs")
    set(CMAKE_C_FLAGS   "-mthumb -mcpu=cortex-m3 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -specs=nano.specs")
    set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3 -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs")
endif()

# Helper to define firmware targets.
# TARGET should end in .elf.
# Will automatically add dependency on behavior runner.
function(add_firmware TARGET SOURCES)
    add_executable(${TARGET} ${SOURCES})
    target_link_libraries(${TARGET} behavior_runner)
    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS
            "-T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/STM32L151x8.ld -T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/rules.ld -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=${TARGET}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")
endfunction()


# Download and unpack googletest at configure time
if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
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
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_subdirectory(embedded)
else()
    add_subdirectory(emulator)
endif()

add_subdirectory(api)
add_subdirectory(projects)