add_library(wrappers
        rgb_led_wrapper.cpp rgb_led_wrapper.h)

target_link_libraries(wrappers
  api
  kl_lib
)

target_include_directories(wrappers
  PUBLIC
    ..
)
