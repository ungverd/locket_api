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

    for (const auto& packet: rx_table.Raw()) {
        logger->log("received packet with id %d", packet.id);
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

void IntegrationTestingBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    logger->log("Button %d pressed, long press: %d", button_index, long_press);
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

void IntegrationTestingBehavior::OnUartCommand(UartCommand& command) {
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

    if (command.NameIs("emit_once")) {
        std::optional<int32_t> id = command.GetNext();
        if (id.has_value()) {
            IdOnlyState s = {static_cast<uint32_t>(id.value())};
            radio->Transmit(s);
            logger->log("ack emit_once");
        } else {
            logger->log("not enough parameters!");
        }
        return;
    }

    if (command.NameIs("emit_beacon")) {
        std::optional<int32_t> id = command.GetNext();
        if (id.has_value()) {
            IdOnlyState s = {static_cast<uint32_t>(id.value())};
            radio->SetBeaconPacket(s);
            logger->log("ack emit_beacon");
        } else {
            logger->log("not enough parameters!");
        }
        return;
    }

    if (command.NameIs("eeprom_write")) {
        std::optional<uint32_t> address = command.GetNext();
        std::optional<uint32_t> value = command.GetNext();
        if (address.has_value() && value.has_value()) {
            if (eeprom->Write(address.value(), value.value())) {
                logger->log("failed to write to EEPROM");
            } else {
                logger->log("EEPROM write success");
            }
        } else {
            logger->log("not enough parameters!");
        }
        return;
    }

    if (command.NameIs("eeprom_read")) {
        std::optional<uint32_t> address = command.GetNext();
        if (address.has_value()) {
            logger->log("value in EEPROM: %d", eeprom->Read<uint32_t>(address.value()));
        } else {
            logger->log("not enough parameters!");
        }
        return;
    }

}
