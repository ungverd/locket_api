add_library(basic_behavior basic_behavior.cpp)
target_link_libraries(basic_behavior api)

add_library(giganda_behaviour try_giganda.h try_giganda.cpp)
target_link_libraries(giganda_behaviour api)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_executable(basic_behavior_emulator basic_behavior_emulator.cpp)
    target_link_libraries(basic_behavior_emulator basic_behavior emulator)

    add_executable(giganda_behavior_emulator giganda_behavior_emulator.cpp)
    target_link_libraries(giganda_behavior_emulator giganda_behaviour emulator)
else()
    set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m3 -std=gnu++14 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -g -fstack-usage -Wall -fno-threadsafe-statics -MMD -MP -specs=nano.specs")
    set(CMAKE_C_FLAGS   "-mthumb -mcpu=cortex-m3 -std=gnu11 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -specs=nano.specs")
    set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3 -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs")

    add_executable(giganda.elf giganda_main.cpp)
    target_link_libraries(giganda.elf giganda_behaviour behavior_runner)

    set_target_properties(giganda.elf PROPERTIES LINK_FLAGS
            "-T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/STM32L151x8.ld -T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/rules.ld -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=giganda.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")

    add_executable(basic_behavior.elf basic_behavior_firmware.cpp)
    target_link_libraries(basic_behavior.elf basic_behavior behavior_runner)

    set_target_properties(basic_behavior.elf PROPERTIES LINK_FLAGS
            "-T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/STM32L151x8.ld -T ${CMAKE_CURRENT_SOURCE_DIR}/../embedded/rules.ld -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=basic_behavior.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")

endif()
