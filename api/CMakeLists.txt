add_library(api INTERFACE)
target_sources(api INTERFACE
        common_states.h
        behavior.h
        led.h
        logger.h
        pill_manager.h
        utility.h
        utility.cpp
)
target_include_directories(api INTERFACE .)