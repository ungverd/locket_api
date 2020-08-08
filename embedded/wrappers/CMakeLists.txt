add_library(wrappers
        rgb_led_wrapper.cpp rgb_led_wrapper.h
        logger_wrapper.cpp logger_wrapper.h
        pill_manager_wrapper.h
        beeper_wrapper.cpp beeper_wrapper.h
        vibro_wrapper.cpp vibro_wrapper.h)

target_link_libraries(wrappers
  api
  kl_lib
)

target_include_directories(wrappers
  PUBLIC
    ..
)
