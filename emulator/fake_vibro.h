#ifndef LOCKET_API_FAKE_VIBRO_H
#define LOCKET_API_FAKE_VIBRO_H

#include "api/vibro.h"
#include "sequenceable.h"

class FakeVibro: public Vibro, public Sequenceable<VibroChunk> {
public:
    void StartOrRestart(const VibroChunk* sequence) override { Sequenceable::StartOrRestart(sequence); }
    void Setup(const VibroChunk& current) override;
    void Off() override;
};


#endif //LOCKET_API_FAKE_VIBRO_H
