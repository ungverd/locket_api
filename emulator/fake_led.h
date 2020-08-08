#ifndef LOCKET_API_EMULATOR_FAKE_LED_H
#define LOCKET_API_EMULATOR_FAKE_LED_H

#include <mutex>
#include <thread>
#include "led.h"
#include "sequenceable.h"

class FakeLed: public RgbLed, public Sequenceable<LedRGBChunk> {
public:
    void StartOrRestart(const LedRGBChunk* sequence) override { Sequenceable::StartOrRestart(sequence); }
    void Setup(const LedRGBChunk& current) override;
    void Off() override;
};

#endif //LOCKET_API_EMULATOR_FAKE_LED_H
