#include "springwind_behavior.h"
#include "utility.h"
#include "sequences.h"

const LedRGBChunk kPlayerStart[] = {
        {{ChunkType::kSetup, {300}}, kExtraLightGreen},
        {{ChunkType::kWait, {300}}},
        {{ChunkType::kEnd}},
};

const LedRGBChunk kMasterStart[] = {
        {{ChunkType::kSetup, {300}}, kExtraLightMagenta},
        {{ChunkType::kWait, {300}}},
        {{ChunkType::kEnd}},
};

const LedRGBChunk kLadyLuStart[] = {
        {{ChunkType::kSetup, {300}}, kExtraLightBlue},
        {{ChunkType::kWait, {300}}},
        {{ChunkType::kEnd}},
};

LedRGBChunk kPlayerCurrent[] = {
        {{ChunkType::kSetup, {0}}, kExtraLightGreen},
        {{ChunkType::kWait, {1000}}},
        {{ChunkType::kEnd}},
};

void WindsBehavior::OnStarted() {
    logger->log("Started execution!");
    radio->SetPowerLevel(RadioPowerLevel::MINUS_15_DBM);
}

void WindsBehavior::EverySecond() {

    // receiving sos
    if (mode == Mode::kMasterSos and rx_table.HasPacketWithId(sos_id)) {
        vibro->StartOrRestart(kBrrBrrBrr);
        rx_table.Clear();
    }

    //sending sos
    if (mode == Mode::kMasterSos and state == State::kActivated) {
        seconds_counter++;
        if (seconds_counter % 3 == 0) {
            seconds_counter = 0;
            radio->ClearBeaconPacket();
            state = State::kIdle;
        }
    }

    //timer changed for player
    if (mode == Mode::kPlayer and state != State::kIdle) {
        seconds_counter++;
        if (seconds_counter >= current_threshold) {
            current_threshold = 0;
            state = State::kIdle;
            seconds_counter = 0;
            vibro->StartOrRestart(kBrrBrrBrr);
            led->StartOrRestart(kPlayerStart);
        } else {
            led->Stop();
            uint8_t red_part = 255*(current_threshold - seconds_counter)/current_threshold;
            uint8_t green_part = 255 - red_part;
            Color current_color = Color(red_part, green_part, 0);
            kPlayerCurrent[0].color = current_color;
            led->StartOrRestart(kPlayerCurrent);
        }

    }
    if (mode == Mode::kLadyLu and state == State::kActivated) {
        seconds_counter ++;
        if (seconds_counter >= LadyLuThreshold) {
            seconds_counter = 0;
            radio->ClearBeaconPacket();
            state = State::kIdle;
        }
    }
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
    RangeLevel = IdToRadioEnum(range);
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
    switch (mode) {
        case Mode::kMasterSos:
            seconds_counter = 0;
            state = State::kActivated;
            radio->SetBeaconPacket({sos_id});
            break;
        case Mode::kLadyLu:
            if (long_press) {
                state = State::kActivated;
                radio ->SetBeaconPacket({path_id});
            }
            break;
        case Mode::kPlayer:
            if (long_press) {
                switch (button_index) {
                    case 0:
                        state = State::kFive;
                        current_threshold = FiveMinThreshold;
                        seconds_counter = 0;
                        break;
                    case 1:
                        state = State::kFifteen;
                        seconds_counter = 0;
                        current_threshold = FifteenThreshold;
                        break;
                    case 2:
                        state = State::kHalfAnHour;
                        seconds_counter = 0;
                        current_threshold = HalfAnHourThreshold;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
}

void WindsBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    logger->log("Received radio packet with id = %d", packet.id);
    if (mode == Mode::kMasterSos) {
        rx_table.AddPacket(packet);
    }
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
