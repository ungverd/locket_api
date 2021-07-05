#ifndef LOCKET_API_RAD_BEHAVIOR_H
#define LOCKET_API_RAD_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"



class RadBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnButtonPressed(uint16_t button_index, bool long_press) override;
    void Flash(unsigned int R, unsigned int G, unsigned int B, unsigned int Timeout);

};

#endif //LOCKET_API_SM_BEHAVIOR_H
