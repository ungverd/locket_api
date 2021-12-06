#include "leo_behavior.h"
#include "utility.h"

LedRGBChunk controlChunk[] = {
    {ChunkType::kSetup, 0, {10, 0, 10}},
    {ChunkType::kWait, 500},
    {ChunkType::kSetup, 0, {0, 0, 0}},
    {ChunkType::kWait, 500},
    {ChunkType::kGoto, 0}
};

void LeoBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
}

void LeoBehavior::EverySecond() {
    if(mode == Mode::lamp) {
        if (rx_table.HasPacketWithId(1)) {
            auto packet = rx_table.GetPacketById(1);
            LedRGBChunk seq[] = {
                {ChunkType::kWait, packet->delay},
                {ChunkType::kSetup, 0, {packet->r, packet->g, packet->b}},
                {ChunkType::kEnd}
            };
            led->StartOrRestart(seq);
        }
        if (rx_table.HasPacketWithId(2)) {
            led->Stop();
        }
        rx_table.Clear();
    }
}

void LeoBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    if (GetSwitchState(dip_value_mask, 1)) {
        mode = Mode::lamp;
        led->Stop();
    } else {
        mode = Mode::control;
        led->StartOrRestart(controlChunk)
    }
}

void LeoBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    if(mode == Mode::control) {
        if(!long_press) {
            if (button_index == 0 || button_index == 2) {
                if ((rand() % 255) < probability) {
                    radio->Transmit({1, r, g, b, delay});
                }
            } else {
                radio->Transmit({2, 0, 0, 0, 0});
            }
        }
    }
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

void LeoBehavior::OnRadioPacketReceived(const IdAndLampParameters& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}

void LeoBehavior::OnUartCommand(UartCommand& command) {
   if (command.NameIs("ping")) {
       logger->log("ack 0");
       return;
   }

    if (command.NameIs("color")) {
        std::optional<int32_t> red = command.GetNext();
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
