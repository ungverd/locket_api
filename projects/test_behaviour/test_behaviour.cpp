//
// Created by juice on 18.07.2021.
//

#include "test_behaviour.h"
#include "utility.h"

const LedRGBChunk kButtonSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
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

void TestBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
    radio->SetBeaconPacket({3});
}

void TestBehavior::EverySecond() {
    if (rx_table.HasPacketWithId(2)) {
        vibro->StartOrRestart(kBrrBrrBrr);
    }
    rx_table.Clear();
}

void TestBehavior::OnPillConnected(PillManager<IdOnlyState>* manager) {
    pill_manager = manager;
    //logger->log("Read value %d from pill", pill_manager->ReadPill().id);
    //led->StartOrRestart(kPillConnectedSequence);
}

void TestBehavior::OnPillDisconnected() {
    logger->log("Pill was disconnected");
    //pill_manager = nullptr;
    //led->StartOrRestart(kPillDisconnectedSequence);
}

void TestBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    vibro->StartOrRestart(kBrrBrr);
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

void TestBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    led->StartOrRestart(kButtonSequence);
}

void TestBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}

void TestBehavior::OnUartCommand(UartCommand& command) {
    if (command.NameIs("ping")) {
        logger->log("pong");
        return;
    }

    if (command.NameIs("plus")) {
        std::optional<int32_t> a = command.GetNext();
        std::optional<int32_t> b = command.GetNext();
        if (a.has_value() && b.has_value()) {
            logger->log("sum equals %d", a.value() + b.value());
        } else {
            logger->log("not enough parameters!");
        }
        return;
    }
}
