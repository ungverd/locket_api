#include "rgb_led_wrapper.h"

void RgbLedWrapper::StartOrRestart(const LedRGBChunk* sequence) {
    implementation->StartOrRestart(sequence);
}
