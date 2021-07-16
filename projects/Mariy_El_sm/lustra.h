//
// Created by juice on 11.07.2021.
//

#ifndef LOCKET_API_LUSTRA_H
#define LOCKET_API_LUSTRA_H

#include "rx_table.h"
#include "behavior.h"

class LustraBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;
    void OnStarted() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;

private:
    RadioPowerLevel range_level = RadioPowerLevel::MINUS_6_DBM;
};


#endif //LOCKET_API_LUSTRA_H
