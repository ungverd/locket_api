set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(emulator
        emulator.h
        fake_led.h
        fake_led.cpp
        fake_pill_manager.h
        fake_pill_manager.cpp
        stdout_logger.cpp stdout_logger.h)
target_link_libraries(emulator Threads::Threads api)
target_include_directories(emulator INTERFACE .)