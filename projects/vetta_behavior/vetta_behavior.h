#ifndef LOCKET_API_VETTA_BEHAVIOR_H
#define LOCKET_API_VETTA_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"


class VettaBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:

    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;
    void OnButtonPressed(uint16_t button_index, bool long_press) override;
    void OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) override;

private:
    RxTable<IdOnlyState> rx_table;
    uint16_t dip_value;
    std::vector<IdOnlyState> ids;
};

#endif //LOCKET_API_VETTA_BEHAVIOR_H
