#include "vetta_behavior.h"
#include "utility.h"

void VettaBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
}

void VettaBehavior::EverySecond() {
    ids = rx_table.Raw();
    uint8_t max_id = 0;
    for(auto& val: ids) {
        if(val.id > max_id) {
            max_id = val.id;
        }
    }
    rx_table.Clear();
    if(max_id > 0) {
        LedRGBChunk seq[4 * max_id];
        for(int i=0; i<max_id; i++) {
            seq[i*4] = {ChunkType::kSetup, 0, kWhite};
            seq[i*4 + 1] = {ChunkType::kWait, 207};
            seq[i*4 + 2] = {ChunkType::kSetup, 0, kBlack};
            seq[i*4 + 3] = {ChunkType::kWait, 207};
        }
        seq[4*max_id - 1] = {ChunkType::kEnd};
        led->StartOrRestart(seq);
    }
}

void VettaBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    dip_value = dip_value_mask;
    logger->log("%d", dip_value);
}

void VettaBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    if (!long_press) {
        logger->log("1");
        LedRGBChunk seq[4 * dip_value];
        logger->log("2");
        for(int i=0; i<dip_value; i++) {
            seq[i*4] = {ChunkType::kSetup, 0, kWhite};
            seq[i*4 + 1] = {ChunkType::kWait, 207};
            seq[i*4 + 2] = {ChunkType::kSetup, 0, kBlack};
            seq[i*4 + 3] = {ChunkType::kWait, 207};
        }
        logger->log("3");
        seq[4*dip_value - 1] = {ChunkType::kEnd};
        logger->log("4");
        led->StartOrRestart(seq);
        logger->log("5");
        radio->Transmit({dip_value});
        logger->log("6");
    }
}

void VettaBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}
