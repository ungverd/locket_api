add_library(api INTERFACE)
target_sources(api INTERFACE
        beeper.h
        color.h
        common_states.h
        behavior.h
        led.h
        logger.h
        pill_manager.h
        sequences.h
        utility.h
        utility.cpp
        vibro.h
        rx_table.h
        radio.h
)
target_include_directories(api INTERFACE . ..)

add_library(qhsm INTERFACE)
target_sources(api INTERFACE
        qhsm.h
        qhsm.c
)
target_include_directories(api INTERFACE .)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    add_executable(api_test
            rx_table_test.cpp)
    target_link_libraries(api_test api gtest_main)
    gtest_discover_tests(api_test)
endif()