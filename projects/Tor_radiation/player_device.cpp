#include "player_device.h"
#include "Glue.h"
#include "health.h"
#include "sequences.h"
#include "utility.h"
#include <cstdlib>

#define UNIQ_ID_BASE    0x1FF80050

// Seed pseudo-random generator with new seed
static inline void Seed(unsigned int Seed) { srand(Seed); }

static inline uint32_t GetUniqID3() {
    return *((uint32_t*)(UNIQ_ID_BASE + 0x14));
}

LedRGBChunk kHealthSequence[] = {
        {{ChunkType::kSetup, {0}}, kExtraLightGreen},
        {{ChunkType::kWait, {100}}},
        {{ChunkType::kGoto, {0}}},
};

const VibroChunk kLongBrr[] = {
        {{ChunkType::kSetup, {kVibroVolume}}},
        {{ChunkType::kWait, {1000}}},
        {{ChunkType::kSetup, {0}}},
        {{ChunkType::kWait, {kVibroRepeatPeriod}}},
        {{ChunkType::kEnd}}
};

void RadBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kHealthSequence);
    vibro->StartOrRestart(kBrrBrrBrr);
    Seed(GetUniqID3());
    Health_ctor(this, this->eeprom, this->logger);
    QMSM_INIT(the_health, (QEvt *)nullptr);

}

void RadBehavior::EverySecond() {
    healthQEvt e;
    //time events
    ++s_counter;
    e.super.sig = TIME_TICK_1S_SIG;
    QMsm_dispatch(the_health, &(e.super));
    if (s_counter % 60 == 0) {
        s_counter = 0;
        e.super.sig = TIME_TICK_1M_SIG;
        QMsm_dispatch(the_health, &(e.super));
    }
    //radio events
    int LustraNear[16] = {};
    for (const IdOnlyState &packet: rx_table.Raw()) {
        LustraNear[packet.id] += 1;
    }
    for (const int lustra_id: LustraNear) {
        if (lustra_id > 0) {
            logger->log("Radiation");
            e.super.sig = RAD_RECEIVED_SIG;
            QMsm_dispatch(the_health, &(e.super));
            logger->log("Radiation dispatched");
        }
    }
    rx_table.Clear();
}

void RadBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    healthQEvt e;
    logger->log("Button Pressed %i %i", button_index, long_press );
    //only long presses registrated
    if (long_press) {
        //middle button for death signal, others for god mode
        if (button_index == 1) {
            logger->log("Btn reset pressed");
            e.super.sig = BTN_RESET_SIG;
        }
        QMsm_dispatch(the_health, &(e.super));
    }

}

void RadBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
    uint8_t first_range = GetSwitchState(dip_value_mask, 5);
    uint8_t second_range = GetSwitchState(dip_value_mask, 6);
    uint8_t third_range = GetSwitchState(dip_value_mask, 7);
    uint8_t fourth_range = GetSwitchState(dip_value_mask, 8);
    uint8_t range = 8 * first_range + 4 * second_range + 2 * third_range + fourth_range;
    if (range > 11) {
        range = 11;
    }
    range_level = IdToRadioEnum(range);
    radio->SetPowerLevel(range_level);
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


void RadBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    rx_table.AddPacket(packet);
}

void RadBehavior::LongVibro() {
    vibro->StartOrRestart(kLongBrr);
}

void RadBehavior::SetColor(Color color) {
    led->Stop();
    kHealthSequence[0].color = color;
    led->StartOrRestart(kHealthSequence);
}