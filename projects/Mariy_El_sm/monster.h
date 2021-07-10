//
// Created by juice on 10.07.2021.
//

#ifndef LOCKET_API_MONSTER_H
#define LOCKET_API_MONSTER_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"



class MonsterBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;
    void OnStarted() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;

private:
    const uint32_t monster_id = 2;
    RadioPowerLevel range_level = RadioPowerLevel::MINUS_6_DBM;
};

#endif //LOCKET_API_MONSTER_H
