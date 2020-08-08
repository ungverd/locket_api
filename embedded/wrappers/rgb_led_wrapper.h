#ifndef LOCKET_API_RGB_LED_WRAPPER_H
#define LOCKET_API_RGB_LED_WRAPPER_H

#include "kl_lib/led.h"
#include "api/led.h"

class RgbLedWrapper: public RgbLed {
public:
    explicit RgbLedWrapper(LedRGBwPower_t* implementation): implementation(implementation) {}
    void StartOrRestart(const LedRGBChunk* sequence) override;
    void StartOrContinue(const LedRGBChunk *sequence) override;
    void Stop() override;

private:
    LedRGBwPower_t* const implementation;
};


#endif //LOCKET_API_RGB_LED_WRAPPER_H
