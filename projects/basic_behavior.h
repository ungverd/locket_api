#ifndef LOCKET_API_BASIC_BEHAVIOR_H
#define LOCKET_API_BASIC_BEHAVIOR_H

#include "pill_manager.h"
#include "behavior.h"

class BasicBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnPillConnected(PillManager<IdOnlyState>* manager) override;
    void OnPillDisconnected() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;
    void OnButtonPressed(uint16_t button_index) override;

private:
    PillManager<IdOnlyState>* pill_manager = nullptr;
    uint32_t seconds_counter = 0;
};

#endif //LOCKET_API_BASIC_BEHAVIOR_H
