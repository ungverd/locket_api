#ifndef LOCKET_API_FAKE_BEEPER_H
#define LOCKET_API_FAKE_BEEPER_H


#include "api/beeper.h"
#include "sequenceable.h"

class FakeBeeper: public Beeper, public Sequenceable<BeepChunk> {
public:
    void StartOrRestart(const BeepChunk* sequence) override { Sequenceable::StartOrRestart(sequence); }
    void Setup(const BeepChunk& current) override;
    void Off() override;
};

#endif //LOCKET_API_FAKE_BEEPER_H
