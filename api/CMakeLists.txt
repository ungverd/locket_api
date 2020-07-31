add_library(api INTERFACE)
target_sources(api INTERFACE
        common_states.h
        behavior.h
        pill_manager.h
)
target_include_directories(api INTERFACE .)