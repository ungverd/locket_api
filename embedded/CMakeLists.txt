set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m3 -std=gnu++14 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -g -fstack-usage -Wall -fno-threadsafe-statics -MMD -MP -specs=nano.specs")
set(CMAKE_C_FLAGS   "-mthumb -mcpu=cortex-m3 -std=gnu11 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -specs=nano.specs")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3 -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs")

add_subdirectory(os)
add_subdirectory(kl_lib)
add_subdirectory(Radio)
add_subdirectory(Pill)
add_subdirectory(wrappers)

add_library(behavior_runner
        behavior_runner.h
        )

target_include_directories(behavior_runner
        PUBLIC .
        )

target_link_libraries(behavior_runner
        wrappers
        kl_lib
        radio
        pill
)
