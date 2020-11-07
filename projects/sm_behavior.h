#ifndef LOCKET_API_SM_BEHAVIOR_H
#define LOCKET_API_SM_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"


class StateMachineBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
};

#endif //LOCKET_API_SM_BEHAVIOR_H
