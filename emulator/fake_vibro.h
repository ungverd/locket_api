#ifndef LOCKET_API_FAKE_VIBRO_H
#define LOCKET_API_FAKE_VIBRO_H

#include "api/vibro.h"

class FakeVibro: public Vibro {
public:
    void StartOrRestart(const BaseChunk *sequence) override;
};


#endif //LOCKET_API_FAKE_VIBRO_H
