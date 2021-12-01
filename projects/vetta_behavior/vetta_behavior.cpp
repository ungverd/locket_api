#include "vetta_behavior.h"
#include "utility.h"

const LedRGBChunk one[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

const LedRGBChunk two[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

const LedRGBChunk three[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

const LedRGBChunk four[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 125},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

void VettaBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
    counter = 0;
    receivedPreviousSecond = false;
}

void VettaBehavior::EverySecond() {
    if(receivedPreviousSecond) {
        rx_table.Clear();
        receivedPreviousSecond = false;
    } else {
        ids = rx_table.Raw();
        if(ids.size() > 0) {
            receivedPreviousSecond = true;
            uint8_t max_id = 0;
            for (auto& val: ids) {
                if (val.id > max_id) {
                    max_id = val.id;
                }
            }
            rx_table.Clear();
            counter = max_id;
        }
    }
    switch(counter) {
        case 0:
            break;
        case 1:
            led->StartOrRestart(one);
            counter -= 1;
            break;
        case 2:
            led->StartOrRestart(two);
            counter -= 2;
            break;
        case 3:
            led->StartOrRestart(three);
            counter -= 3;
            break;
        default:
            led->StartOrRestart(four);
            counter -= 4;
            break;
    }
}

void VettaBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    dip_value = dip_value_mask;
    logger->log("%d", dip_value);
}

void VettaBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    if (!long_press) {
        logger->log("5");
        radio->Transmit({dip_value});
        logger->log("6");
    }
}

void VettaBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}
