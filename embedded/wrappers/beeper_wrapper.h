#ifndef LOCKET_API_BEEPER_WRAPPER_H
#define LOCKET_API_BEEPER_WRAPPER_H

#include "kl_lib/beeper.h"
#include "api/beeper.h"

class BeeperWrapper: public Beeper {
public:
    explicit BeeperWrapper(Beeper_t* implementation): implementation(implementation) {}
    void StartOrRestart(const BeepChunk* sequence) override;

private:
    Beeper_t* const implementation;
};

#endif //LOCKET_API_BEEPER_WRAPPER_H
