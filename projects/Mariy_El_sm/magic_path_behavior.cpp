#include "magic_path_behavior.h"

const LedRGBChunk kButtonSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

void MagicPathBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
}

void MagicPathBehavior::EverySecond() {
    if (!rx_table.Raw().empty()) {
        led->StartOrRestart(kStartSequence);
    }
    rx_table.Clear();
}

void MagicPathBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    rx_table.AddPacket(packet);
}

