//
// Created by juice on 24.08.2020.
//

#ifndef LOCKET_API_KIRLITS_BEHAVIOR_H
#define LOCKET_API_KIRLITS_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"

enum class LocketEnum{YELLOW, WHITE, BLUE, RED, BLUERED, SILENT};

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
    std::vector<LedRGBChunk> sequence;
    uint8_t LocketId = 1;
    bool silent = false;
};

void CreateSequence(const std::vector<IdAndTypeState>& rx_table, std::vector<LedRGBChunk>& sequence);
bool IfSilentNear(const std::vector<IdAndTypeState>& rx_table);
uint8_t EnumToType(LocketEnum LocketType);
LocketEnum TypeToEnum(uint8_t id);

#endif //LOCKET_API_KIRLITS_BEHAVIOR_H
