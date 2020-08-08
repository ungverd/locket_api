#include "fake_vibro.h"

#include <iostream>

void FakeVibro::Setup(const VibroChunk& current) {
    if (current.volume == 0) {
        Off();
    } else {
        std::cout << "Started vibrating with volume " << current.volume << std::endl;
    }
}

void FakeVibro::Off() {
    std::cout << "Vibro off" << std::endl;
}
