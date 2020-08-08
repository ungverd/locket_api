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
)
target_include_directories(api INTERFACE . ..)