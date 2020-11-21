//
// Created by juice on 21.08.2020.
//

#ifndef LOCKET_API_FEELING_BEHAVIOR_H
#define LOCKET_API_FEELING_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"


#define MASTER 1
#define SLAVE 0

class FeelingBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnRadioPacketReceived(const IdOnlyState& packet) override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;

private:
    uint32_t seconds_counter = 0;
    RxTable<IdOnlyState> rx_table;
    uint8_t LocketType = SLAVE;
    RadioPowerLevel RangeLevel = RadioPowerLevel::MINUS_6_DBM;
};

RadioPowerLevel TypeToEnum(uint8_t level);

#endif //LOCKET_API_FEELING_BEHAVIOR_H
