#include "fake_beeper.h"

#include <iostream>

void FakeBeeper::Setup(const BeepChunk& current) {
    if (current.volume == 0) {
        Off();
    } else {
        std::cout << "Beeping on frequency " << current.freq_hz << " with volume " << current.volume << std::endl;
    }
}

void FakeBeeper::Off() {
    std::cout << "Beeper off" << std::endl;
}

