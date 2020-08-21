//
// Created by juice on 21.08.2020.
//

#ifndef LOCKET_API_FEELING_BEHAVIOR_H
#define LOCKET_API_FEELING_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"

class FeelingBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnRadioPacketReceived(const IdOnlyState& packet) override;

private:
    uint32_t seconds_counter = 0;
    RxTable<IdOnlyState> rx_table;

};


#endif //LOCKET_API_FEELING_BEHAVIOR_H
