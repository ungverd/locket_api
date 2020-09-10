//
// Created by juice on 24.08.2020.
//

#ifndef LOCKET_API_KIRLITS_H
#define LOCKET_API_KIRLITS_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"

enum class LocketEnum{YELLOW, WHITE, SILENT, BLUE, RED, BLUERED};

class KirlitsBehavior: public Behavior<IdOnlyState, IdAndTypeState> {
public:

    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;
    void OnRadioPacketReceived(const IdAndTypeState& packet) override;

private:
    uint32_t seconds_counter = 0;
    RxTable<IdAndTypeState> rx_table;
    LocketEnum LocketType = LocketEnum::YELLOW;
    LocketEnum IdToEnum(uint8_t id);
};

#endif //LOCKET_API_KIRLITS_H
