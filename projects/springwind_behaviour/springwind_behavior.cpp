#include "springwind_behaviour.h"
#include "utility.h"
#include "sequences.h"

const LedRGBChunk kPlayerStart[] = {
        {ChunkType::kSetup, 300, kExtraLightGreen},
        {ChunkType::kWait, 300},
        {ChunkType::kEnd},
};

const LedRGBChunk kMasterStart[] = {
        {ChunkType::kSetup, 300, kExtraLightMagenta},
        {ChunkType::kWait, 300},
        {ChunkType::kEnd},
};

const LedRGBChunk kLadyLuStart[] = {
        {ChunkType::kSetup, 300, kExtraLightBlue},
        {ChunkType::kWait, 300},
        {ChunkType::kEnd},
};

void WindsBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
    radio->SetBeaconPacket({22});
}

void WindsBehavior::EverySecond() {
    ++seconds_counter;
    rx_table.Clear();
}


void WindsBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {

    // get locket type: Player, Master or Lady Lu
    uint8_t first_type = GetSwitchState(dip_value_mask, 1);
    uint8_t second_type = GetSwitchState(dip_value_mask, 2);
    switch (2*first_type + second_type) {
        case 0:
            mode = Mode::kPlayer;
            state = State::kIdle;
            led->StartOrRestart(kPlayerStart);
            break;
        case 1:
            mode = Mode::kMasterSos;
            led->StartOrRestart(kMasterStart);
            break;
        case 2:
            mode = Mode::kLadyLu;
            led->StartOrRestart(kLadyLuStart);
            break;
        default:
            mode = Mode::kPlayer;
            state = State::kIdle;
            led->StartOrRestart(kPlayerStart);
            break;
    }

    // set radio level
    uint8_t first_range = GetSwitchState(dip_value_mask, 5);
    uint8_t second_range = GetSwitchState(dip_value_mask, 6);
    uint8_t third_range = GetSwitchState(dip_value_mask, 7);
    uint8_t fourth_range = GetSwitchState(dip_value_mask, 8);
    uint8_t range = 8*first_range + 4*second_range + 2*third_range + fourth_range;
    if (range > 11) {
        range = 11;
    }
    RangeLevel = TypeToEnum(range);
    radio->SetPowerLevel(RangeLevel);


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

void WindsBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {

}

void WindsBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    rx_table.AddPacket(packet);
}

void WindsBehavior::OnUartCommand(UartCommand& command) {
   if (command.NameIs("ping")) {
       logger->log("ack 0");
       return;
   }

    if (command.NameIs("version")) {
        logger->log("Wind Of Spring Locket Code based on aeremin locket api for Ostranna Creative Group");
        return;
    }
}
