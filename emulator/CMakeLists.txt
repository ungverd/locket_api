set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(emulator
        emulator.h
        fake_led.h
        fake_led.cpp
        fake_pill_manager.h
        fake_pill_manager.cpp
        fake_radio.h
        stdout_logger.cpp stdout_logger.h
        fake_beeper.cpp fake_beeper.h
        fake_vibro.cpp fake_vibro.h
        sequenceable.h)
target_link_libraries(emulator Threads::Threads api)
target_include_directories(emulator INTERFACE .)

add_executable(emulator_test fake_led_test.cpp)
target_link_libraries(emulator_test emulator gtest_main)
gtest_discover_tests(emulator_test)