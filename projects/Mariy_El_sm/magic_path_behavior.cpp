#include "magic_path_behavior.h"
#include "Glue.h"

const LedRGBChunk kPathSequence[] = {
        {{ChunkType::kSetup, {1000}}, kRed},
        {{ChunkType::kWait, {1000}}},
        {{ChunkType::kGoto, {1}}},
};

const LedRGBChunk kStopSequence[] = {
        {{ChunkType::kSetup, {1000}}, kBlack},
        {{ChunkType::kWait, {1000}}},
        {{ChunkType::kEnd}},
};

void MagicPathBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void MagicPathBehavior::EverySecond() {
    ++count;
    if (count % 4) {
        count = 0;
        if (rx_table.HasPacketWithId(path_id)) {
            led->StartOrRestart(kPathSequence);
        } else {
            led->StartOrRestart(kStopSequence);
        }
    }
    rx_table.Clear();
}

void MagicPathBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    rx_table.AddPacket(packet);
}

