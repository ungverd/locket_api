#include "magic_path_behavior.h"
#include "Glue.h"

const LedRGBChunk kPathSequence[] = {
        {{ChunkType::kSetup, {100}}, kBlue},
        {{ChunkType::kWait, {1000}}},
        {{ChunkType::kGoto, {1}}},
};

void MagicPathBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void MagicPathBehavior::EverySecond() {
    if (rx_table.HasPacketWithId(path_id)) {
        led->StartOrRestart(kPathSequence);
    } else {
        led->Stop();
    }
    rx_table.Clear();
}

void MagicPathBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    rx_table.AddPacket(packet);
}

