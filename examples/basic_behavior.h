#ifndef LOCKET_API_BASIC_BEHAVIOR_H
#define LOCKET_API_BASIC_BEHAVIOR_H

#include "behavior.h"
#include "pill_manager.h"
#include <iostream>

class BasicBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnPillConnected(PillManager<IdOnlyState>* manager) override;
    void OnPillDisconnected() override;

private:
    PillManager<IdOnlyState>* pill_manager;
};

#endif //LOCKET_API_BASIC_BEHAVIOR_H
