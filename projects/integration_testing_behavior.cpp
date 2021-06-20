#include "integration_testing_behavior.h"
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

const LedRGBChunk kPillConnectedSequence[] = {
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

const LedRGBChunk kPillDisconnectedSequence[] = {
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

const VibroChunk kVibroDoNothing[] = {{ChunkType::kEnd}};
const BeepChunk kBeeperDoNothing[] = {{ChunkType::kEnd}};

const VibroChunk* kVibroByMode[] = {kBrrBrr, kVibroDoNothing};
const BeepChunk* kBeeperByMode[] = {kShortBeep, kBeeperDoNothing};

void IntegrationTestingBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
}

void IntegrationTestingBehavior::EverySecond() {
    ++seconds_counter;
    if (seconds_counter % 30 == 0) {
        logger->log("30 seconds passed");
    }
    if (seconds_counter % 5 == 0) {
        led->StartOrRestart(kStartSequence);
    }

    if (seconds_counter % 5 == 0) {
        led->StartOrRestart(kStartSequence);
    }

    if (rx_table.HasPacketWithId(17)) {
        vibro->StartOrRestart(kBrrBrrBrr);
    }
    rx_table.Clear();
}

void IntegrationTestingBehavior::OnPillConnected(PillManager<IdOnlyState>* manager) {
    pill_manager = manager;
    logger->log("Read value %d from pill", pill_manager->ReadPill().id);
    led->StartOrRestart(kPillConnectedSequence);
}

void IntegrationTestingBehavior::OnPillDisconnected() {
    logger->log("Pill was disconnected");
    pill_manager = nullptr;
    led->StartOrRestart(kPillDisconnectedSequence);
}

void IntegrationTestingBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
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

void IntegrationTestingBehavior::OnButtonPressed(uint16_t button_index) {
    if (button_index == 0) {
        radio->Transmit({17});
    }

    if (button_index == 1) {
        led->StartOrRestart(kButtonSequence);
    }

    if (button_index == 2) {
        mode = Mode(1 - static_cast<int>(mode));
    }
}

void IntegrationTestingBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    rx_table.AddPacket(packet);
}
