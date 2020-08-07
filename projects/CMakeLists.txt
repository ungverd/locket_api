add_library(basic_behavior basic_behavior.cpp)
target_link_libraries(basic_behavior api)

add_library(giganda_behaviour giganda_behavior.h giganda_behavior.cpp)
target_link_libraries(giganda_behaviour api)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_executable(basic_behavior_emulator basic_behavior_emulator.cpp)
    target_link_libraries(basic_behavior_emulator basic_behavior emulator)

    add_executable(giganda_behavior_emulator giganda_behavior_emulator.cpp)
    target_link_libraries(giganda_behavior_emulator giganda_behaviour emulator)
else()
    add_firmware(giganda.elf giganda_firmware.cpp)
    target_link_libraries(giganda.elf giganda_behaviour)

    add_firmware(basic_behavior.elf basic_behavior_firmware.cpp)
    target_link_libraries(basic_behavior.elf basic_behavior)
endif()
