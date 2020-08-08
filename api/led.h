#ifndef LOCKET_API_LED_H
#define LOCKET_API_LED_H

#include "sequences.h"

// Interface allowing to control RGB LED by using LedRGBChunk sequences.
// Reading general documentation on sequences in sequences.h first is *highly* recommended.
class RgbLed {
public:
    // Starts execution of the sequence. If same sequence is already set to be executed by the previous call - will
    // start it from the beginning.
    // Returns instantaneously, even if sequence have waits (sequences are always executed in parallel with main
    // program flow!).
    virtual void StartOrRestart(const LedRGBChunk* sequence) = 0;

    // Starts execution of the sequence. If same sequence is already set to be executed by the previous call - have
    // no effect.
    // Returns instantaneously, even if sequence have waits (sequences are always executed in parallel with main
    // program flow!).
    virtual void StartOrContinue(const LedRGBChunk* sequence) = 0;

    // Stop sequence execution (or does nothing if there is no active sequence).
    // Turns LED off.
    virtual void Stop() = 0;

    virtual ~RgbLed() {}; // Abstract classes must have virtual destructor.
};

#endif //LOCKET_API_LED_H
