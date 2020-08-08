#ifndef LOCKET_API_FAKE_BEEPER_H
#define LOCKET_API_FAKE_BEEPER_H


#include "api/beeper.h"

class FakeBeeper: public Beeper {
public:
    void StartOrRestart(const BeepChunk *sequence) override;
};


#endif //LOCKET_API_FAKE_BEEPER_H
