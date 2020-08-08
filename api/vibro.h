#ifndef LOCKET_API_VIBRO_H
#define LOCKET_API_VIBRO_H

#include "sequences.h"

// Interface allowing to control vibro.
class Vibro {
public:
    virtual ~Vibro() {};
    virtual void StartOrRestart(const BaseChunk* sequence) = 0;
};


#endif //LOCKET_API_VIBRO_H
