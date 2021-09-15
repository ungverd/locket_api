//
// Created by juice on 15.09.2021.
//

//
// Created by juice on 21.08.2020.
//
#include "seven_strides_behaviour.h"
#include "utility.h"

const unsigned int WorkLightLengthMS = 120000;
const unsigned int WorkTransmitLengthS = 180;

const LedRGBChunk kBackgroundRXSequence[] = {
        {{ChunkType::kSetup, {200}}, kExtraLightRed},
        {{ChunkType::kWait, {299}}},
        {{ChunkType::kGoto, {1}}}
};

const LedRGBChunk kBackgroundTXSequence[] = {
        {{ChunkType::kSetup, {200}}, kExtraLightBlue},
        {{ChunkType::kWait, {299}}},
        {{ChunkType::kGoto, {1}}}
};

const LedRGBChunk kBackgroundRXTXSequence[] = {
        {{ChunkType::kSetup, {200}}, kExtraLightMagenta},
        {{ChunkType::kWait, {299}}},
        {{ChunkType::kGoto, {1}}}
};

const LedRGBChunk kWorkingSequence[] = {
        {{ChunkType::kSetup, {300}}, kGreen},
        {{ChunkType::kWait, {WorkLightLengthMS}}},
        {{ChunkType::kGoto, {1}}}
};


void SevenStridesBehavior::OnStarted() {
    logger->log("Started execution!");
}

void SevenStridesBehavior::EverySecond() {
    if ((LocketType != LOCKET_RX) and working) {
        tx_seconds_counter++;
        if (tx_seconds_counter >= 180) {
            radio->ClearBeaconPacket();
            working = false;
            tx_seconds_counter = 0;
        }
    }
    if (LocketType != LOCKET_TX and not working) {
        rx_seconds_counter++;
        if (rx_seconds_counter % 3 == 0) {
            rx_seconds_counter = 0;
            vibro->StartOrRestart(kBrrBrrBrr);
        }
    }
}

void SevenStridesBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    if (LocketType != LOCKET_TX and not working) {
        rx_table.AddPacket(packet);
    }
}

void SevenStridesBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    if (LocketType != LOCKET_RX) {
        working = true;
        tx_seconds_counter = 0;
        led->StartOrRestart(kWorkingSequence);
        radio->SetBeaconPacket({1});
    }
}

void SevenStridesBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    uint8_t first_type = GetSwitchState(dip_value_mask, 1);
    uint8_t second_type = GetSwitchState(dip_value_mask, 2);
    switch (2*first_type + second_type) {
        case 1:
            LocketType = LOCKET_RX;
            led->StartOrRestart(kBackgroundRXSequence);
            break;
        case 2:
            LocketType = LOCKET_TX;
            led->StartOrRestart(kBackgroundTXSequence);
            break;
        case 3:
            LocketType = LOCKET_RXTX;
            led->StartOrRestart(kBackgroundRXTXSequence);
            break;
        default:
            LocketType = LOCKET_RX;
            led->StartOrRestart(kBackgroundRXSequence);

    }
    uint8_t first_range = GetSwitchState(dip_value_mask, 5);
    uint8_t second_range = GetSwitchState(dip_value_mask, 6);
    uint8_t third_range = GetSwitchState(dip_value_mask, 7);
    uint8_t fourth_range = GetSwitchState(dip_value_mask, 8);
    uint8_t range = 8*first_range + 4*second_range + 2*third_range + fourth_range;
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