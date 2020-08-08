#include "fake_led.h"
#include <chrono>
#include <iostream>

void FakeLed::Setup(const LedRGBChunk& current) {
    if (current.color == kBlack) {
        Off();
    } else {
        std::cout << "Setting LED to " <<
                  uint32_t(current.color.r) << " " <<
                  uint32_t(current.color.g) << " " <<
                  uint32_t(current.color.b) << std::endl;
    }
}

void FakeLed::Off() {
    std::cout << "Turning LED off" << std::endl;
}
