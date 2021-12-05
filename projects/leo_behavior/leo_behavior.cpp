#include "leo_behavior.h"
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

void LeoBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
    radio->SetBeaconPacket({22});
}

void LeoBehavior::EverySecond() {
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


    if (seconds_counter % 10 == 5) {
        vibro->StartOrRestart(kVibroByMode[static_cast<int>(mode)]);
    }

    if (seconds_counter % 10 == 0) {
        beeper->StartOrRestart(kBeeperByMode[static_cast<int>(mode)]);
    }

    if (rx_table.HasPacketWithId(17)) {
        vibro->StartOrRestart(kBrrBrrBrr);
    }
    rx_table.Clear();
}

void LeoBehavior::OnPillConnected(PillManager<IdOnlyState>* manager) {
    pill_manager = manager;
    logger->log("Read value %d from pill", pill_manager->ReadPill().id);
    led->StartOrRestart(kPillConnectedSequence);
}

void LeoBehavior::OnPillDisconnected() {
    logger->log("Pill was disconnected");
    pill_manager = nullptr;
    led->StartOrRestart(kPillDisconnectedSequence);
}

void LeoBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
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

void LeoBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    if (button_index == 0) {
        logger->log("Sent some spam!");
        radio->Transmit({17});
    }

    if (button_index == 1) {
        led->StartOrRestart(kButtonSequence);
    }

    if (button_index == 2) {
        mode = Mode(1 - static_cast<int>(mode));
    }
}

void LeoBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}

void LeoBehavior::OnUartCommand(UartCommand& command) {
   if (command.NameIs("ping")) {
       logger->log("ack 0");
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

    if (command.NameIs("version")) {
        logger->log("Basic Behaviour Locket Api");
        return;
    }
}
