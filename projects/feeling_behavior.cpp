//
// Created by juice on 21.08.2020.
//
#include "feeling_behavior.h"

const LedRGBChunk kFeelSequence[] = {
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};



void FeelingBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void FeelingBehavior::EverySecond() {
    ++seconds_counter;
    if (rx_table.HasPacketWithId(19) && (seconds_counter % 5 == 0)) {
        vibro->StartOrRestart(kBrrBrrBrr);
        led->StartOrRestart(kFeelSequence);
        rx_table.Clear();
        logger->log("BrrBrrBrr");
    }
    radio->Transmit({19});
    logger->log("PacketSent!");

}

void FeelingBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    rx_table.AddPacket(packet);
}
