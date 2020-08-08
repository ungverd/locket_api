#ifndef LOCKET_API_VIBRO_H
#define LOCKET_API_VIBRO_H

#include "sequences.h"

// Interface allowing to control vibro by using VibroChunk sequences.
// Reading general documentation on sequences in sequences.h first is *highly* recommended.
class Vibro {
public:
    // Starts execution of the sequence. If same sequence is already set to be executed by the previous call - will
    // start it from the beginning.
    // Returns instantaneously, even if sequence have waits (sequences are always executed in parallel with main
    // program flow!).
    virtual void StartOrRestart(const VibroChunk* sequence) = 0;

    virtual ~Vibro() {}; // Abstract classes must have virtual destructor.
};


#endif //LOCKET_API_VIBRO_H
