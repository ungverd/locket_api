#include "rgb_led_wrapper.h"

void RgbLedWrapper::StartOrRestart(const LedRGBChunk* sequence) {
    implementation->StartOrRestart(sequence);
}

void RgbLedWrapper::StartOrContinue(const LedRGBChunk* sequence) {
    implementation->StartOrContinue(sequence);
}

void RgbLedWrapper::Stop() {
    implementation->Stop();
}

