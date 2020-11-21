//
// Created by juice on 21.08.2020.
//
#include "feeling_behavior.h"
#include "utility.h"

const LedRGBChunk kMasterStartSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

const LedRGBChunk kSlaveStartSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 299},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

void FeelingBehavior::OnStarted() {
    logger->log("Started execution!");
}

void FeelingBehavior::EverySecond() {
    if (LocketType == SLAVE) {
        ++seconds_counter;
        if (rx_table.HasPacketWithId(1) && (seconds_counter % 10 == 0)) {
            vibro->StartOrRestart(kBrrBrrBrr);
            rx_table.Clear();
            logger->log("BrrBrrBrr");
            seconds_counter = 0;
        }
    }
}

void FeelingBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    rx_table.AddPacket(packet);
}

void FeelingBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    if (GetSwitchState(dip_value_mask, 1)) {
        led->StartOrRestart(kMasterStartSequence);
        radio->SetBeaconPacket({1});
        LocketType = MASTER;
    } else {
        led->StartOrRestart(kSlaveStartSequence);
        radio->ClearBeaconPacket();
        LocketType = SLAVE;
    }
    uint8_t first_range = GetSwitchState(dip_value_mask, 5);
    uint8_t second_range = GetSwitchState(dip_value_mask, 6);
    uint8_t third_range = GetSwitchState(dip_value_mask, 7);
    uint8_t fourth_range = GetSwitchState(dip_value_mask, 8);
    uint8_t range = first_range + 2*second_range + 4*third_range * 8*fourth_range;
    if (range > 11) {
        range = 11;
    }
    RangeLevel = TypeToEnum(range);
    radio->SetPowerLevel(RangeLevel);
    logger->log("DIP switch changed to %d%d%d%d%d%d%d%d",
               GetSwitchState(dip_value_mask, 1),
               GetSwitchState(dip_value_mask, 2),
               GetSwitchState(dip_value_mask, 3),
               GetSwitchState(dip_value_mask, 4),
               GetSwitchState(dip_value_mask, 5),
               GetSwitchState(dip_value_mask, 6),
               GetSwitchState(dip_value_mask, 7),
               GetSwitchState(dip_value_mask, 8));
}


RadioPowerLevel TypeToEnum(uint8_t id) {
    switch (id) {
        case 0:
            return RadioPowerLevel::MINUS_30_DBM;
        case 1:
            return RadioPowerLevel::MINUS_27_DBM;
        case 2:
            return RadioPowerLevel::MINUS_25_DBM;
        case 3:
            return RadioPowerLevel::MINUS_20_DBM;
        case 4:
            return RadioPowerLevel::MINUS_15_DBM;
        case 5:
            return RadioPowerLevel::MINUS_10_DBM;
        case 6:
            return RadioPowerLevel::MINUS_6_DBM;
        case 7:
            return RadioPowerLevel::PLUS_0_DBM;
        case 8:
            return RadioPowerLevel::PLUS_5_DBM;
        case 9:
            return RadioPowerLevel::PLUS_7_DBM;
        case 10:
            return RadioPowerLevel::PLUS_10_DBM;
        case 11:
            return RadioPowerLevel::PLUS_12_DBM;
        default:
            return RadioPowerLevel::PLUS_0_DBM;
    }
}