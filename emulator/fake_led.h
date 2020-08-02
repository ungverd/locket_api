#ifndef LOCKET_API_EMULATOR_FAKE_LED_H
#define LOCKET_API_EMULATOR_FAKE_LED_H

#include "led.h"

class FakeLed: public RgbLed {
public:
    void StartOrRestart(const RgbLedSequence &sequence) override;
};

#endif //LOCKET_API_EMULATOR_FAKE_LED_H
