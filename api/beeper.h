#ifndef LOCKET_API_BEEPER_H
#define LOCKET_API_BEEPER_H

#include "sequences.h"

// Interface allowing to control beeper by using BeepChunk sequences.
// Reading general documentation on sequences in sequences.h first is *highly* recommended.
class Beeper {
public:
    // Starts execution of the sequence. If same sequence is already set to be executed by the previous call - will
    // start it from the beginning.
    // Returns instantaneously, even if sequence have waits (sequences are always executed in parallel with main
    // program flow!).
    virtual void StartOrRestart(const BeepChunk* sequence) = 0;

    // Starts execution of the sequence. If same sequence is already set to be executed by the previous call - have
    // no effect.
    // Returns instantaneously, even if sequence have waits (sequences are always executed in parallel with main
    // program flow!).
    virtual void StartOrContinue(const BeepChunk* sequence) = 0;

    // Stop sequence execution (or does nothing if there is no active sequence).
    // Turns beeper off.
    virtual void Stop() = 0;

    virtual ~Beeper() {}; // Abstract classes must have virtual destructor.
};

#endif //LOCKET_API_BEEPER_H
