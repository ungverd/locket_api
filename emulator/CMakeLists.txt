add_library(emulator
        emulator.h
        emulator.cpp
        fake_led.h
        fake_led.cpp
        fake_pill_manager.h
        fake_pill_manager.cpp
)
target_link_libraries(emulator api)