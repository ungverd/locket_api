#ifndef LOCKET_API_EMULATOR_EMULATOR_H
#define LOCKET_API_EMULATOR_EMULATOR_H

#include "behavior.h"

template <typename PillState = EmptyState, typename RadioPacket = EmptyState>
class Emulator {
public:
    Emulator(Behavior<PillState, RadioPacket>* behavior): behavior(behavior) {};

    void Run();

private:
    Behavior<PillState, RadioPacket>* const behavior;
};

template<typename PillState, typename RadioPacket>
void Emulator<PillState, RadioPacket>::Run() {
    behavior->OnStarted();
}

#endif //LOCKET_API_EMULATOR_EMULATOR_H
