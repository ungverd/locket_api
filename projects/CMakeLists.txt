add_library(basic_behavior basic_behavior/basic_behavior.cpp)
target_link_libraries(basic_behavior api)

add_library(integration_testing_behavior integration_testing_behavior/integration_testing_behavior.cpp)
target_link_libraries(integration_testing_behavior api)

add_library(giganda_behaviour giganda_behavior/giganda_behavior.h giganda_behavior/giganda_behavior.cpp)
target_link_libraries(giganda_behaviour api)

add_library(feeling_behavior feeling_behavior/feeling_behavior.h feeling_behavior/feeling_behavior.cpp)
target_link_libraries(feeling_behavior api)

add_library(kirlits_behavior kirlits_behavior/kirlits_behavior.cpp kirlits_behavior/kirlits_behavior.h)
target_link_libraries(kirlits_behavior api)

add_library(sm_behavior sm_behavior/sm_behavior.cpp sm_behavior/eventHandlers.cpp sm_behavior/ability.cpp)
target_link_libraries(sm_behavior api qhsm)

add_library(Mariy_El_behavior Mariy_El_sm/player_device.cpp Mariy_El_sm/health.cpp Mariy_El_sm/sm_data.cpp
        Mariy_El_sm/monster.cpp Mariy_El_sm/lustra.cpp)
target_link_libraries(Mariy_El_behavior api qhsm)

add_library(Mariy_El_magic_path_behavior Mariy_El_sm/magic_path_behavior.cpp Mariy_El_sm/magic_path_behavior.h)
target_link_libraries(Mariy_El_magic_path_behavior api)

add_library(test_behaviour test_behaviour/test_behaviour.cpp)
target_link_libraries(test_behaviour api)


if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_executable(basic_behavior_emulator basic_behavior/basic_behavior_emulator.cpp)
    target_link_libraries(basic_behavior_emulator basic_behavior emulator)

    add_executable(giganda_behavior_emulator giganda_behavior/giganda_behavior_emulator.cpp)
    target_link_libraries(giganda_behavior_emulator giganda_behaviour emulator)

    add_executable(feeling_behavior_emulator feeling_behavior/feeling_behavior_emulator.cpp)
    target_link_libraries(feeling_behavior_emulator feeling_behavior emulator)

    add_executable(kirlits_behavior_emulator kirlits_behavior/kirlits_behavior_emulator.cpp)
    target_link_libraries(kirlits_behavior_emulator kirlits_behavior emulator)

    add_executable(sm_behavior_emulator sm_behavior/sm_behavior_emulator.cpp)
    target_link_libraries(sm_behavior_emulator sm_behavior emulator)

    add_executable(Mariy_El_emulator Mariy_El_sm/Mariy_El_emulator.cpp)
    target_link_libraries(Mariy_El_emulator Mariy_El_behavior emulator)
else()
    add_firmware(giganda.elf giganda_behavior/giganda_firmware.cpp)
    target_link_libraries(giganda.elf giganda_behaviour)

    add_firmware(basic_behavior.elf basic_behavior/basic_behavior_firmware.cpp)
    target_link_libraries(basic_behavior.elf basic_behavior)

    add_firmware(integration_testing_behavior.elf integration_testing_behavior/integration_testing_behavior_firmware.cpp)
    target_link_libraries(integration_testing_behavior.elf integration_testing_behavior)

    add_firmware(feeling_behavior.elf feeling_behavior/feeling_behavior_firmware.cpp)
    target_link_libraries(feeling_behavior.elf feeling_behavior)

    add_firmware(kirlits_behavior.elf kirlits_behavior/kirlits_behavior_firmware.cpp)
    target_link_libraries(kirlits_behavior.elf kirlits_behavior)

    add_firmware(sm_behavior.elf sm_behavior/sm_behavior_firmware.cpp)
    target_link_libraries(sm_behavior.elf sm_behavior)

    add_firmware(Mariy_El.elf Mariy_El_sm/Mariy_El_firmware.cpp)
    target_link_libraries(Mariy_El.elf Mariy_El_behavior)

    add_firmware(Mariy_El_fd.elf Mariy_El_sm/magic_path_firmware.cpp)
    target_link_libraries(Mariy_El_fd.elf Mariy_El_magic_path_behavior)

    add_firmware(test_behaviour.elf test_behaviour/test_behaviour_firmware.cpp)
    target_link_libraries(test_behaviour.elf test_behaviour)
endif()

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_executable(kirlits_test
            sequence_creation_test.cpp)
    target_link_libraries(kirlits_test kirlits_behavior gtest_main)
    gtest_discover_tests(kirlits_test)
endif()
