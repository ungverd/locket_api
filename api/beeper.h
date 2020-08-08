#ifndef LOCKET_API_BEEPER_H
#define LOCKET_API_BEEPER_H

#include "sequences.h"

// Interface allowing to control beeper.
class Beeper {
public:
    virtual ~Beeper() {};
    virtual void StartOrRestart(const BeepChunk* sequence) = 0;
};

#endif //LOCKET_API_BEEPER_H
