#include "basic_behavior.h"
#include "utility.h"

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


void BasicBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void BasicBehavior::EverySecond() {
    ++seconds_counter;
    if (seconds_counter % 30 == 0) {
        logger->log("30 seconds passed");
    }

    if (seconds_counter % 5 == 0) {
        led->StartOrRestart(kStartSequence);
    }
}

void BasicBehavior::OnPillConnected(PillManager<IdOnlyState>* manager) {
    pill_manager = manager;
    logger->log("Read value %d from pill", pill_manager->ReadPill().id);
}

void BasicBehavior::OnPillDisconnected() {
    logger->log("Pill was disconnected");
    pill_manager = nullptr;
}

void BasicBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    if (GetSwitchState(dip_value_mask, 1)) {
        led->StartOrRestart(kStartSequence);
    } else {
        led->StartOrRestart(kFailureSequence);
    }
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

void BasicBehavior::OnButtonPressed(uint16_t button_index) {
    if (button_index == 1) {
        led->StartOrRestart(kButtonSequence);
    }
}
