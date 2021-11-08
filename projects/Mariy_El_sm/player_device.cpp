#include "player_device.h"
#include "Glue.h"
#include "health.h"
#include "sequences.h"
#include "utility.h"

LedRGBChunk kRadFlashSequence[] = {
        {{ChunkType::kSetup, {0}}, kWhite},
        {{ChunkType::kWait, {500}}},
        {{ChunkType::kSetup, {0}}, kBlack},
        {{ChunkType::kWait, {100}}},
        {{ChunkType::kGoto, {2}}},
};

LedRGBChunk kRadSequence[] = {
        {{ChunkType::kSetup, {0}}, kWhite},
        {{ChunkType::kWait, {100}}},
        {{ChunkType::kGoto, {0}}},
};

const VibroChunk kLongBrr[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, 3000},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, kVibroRepeatPeriod},
        {ChunkType::kEnd}
};

const VibroChunk kLongBrrForGod[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, 1000},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, kVibroRepeatPeriod},
        {ChunkType::kEnd}
};

void RadBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    vibro->StartOrRestart(kBrrBrrBrr);
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
    if (rx_table.HasPacketWithId(monster_id)) {
        logger->log("Monster");
        ++monster_s_counter;
        //every five seconds to avoid too frequent vibro
        if (monster_s_counter % 5 == 0) {
            monster_s_counter = 0;
            e.super.sig = MONSTER_SIGNAl_SIG;
            QMsm_dispatch(the_health, &(e.super));
            logger->log("Monster");
        }
    }
    if (rx_table.HasPacketWithId(rad_id)) {
        logger->log("Radiation");
        ++rad_s_counter;
        //every five seconds to avoid too frequent vibro
        if (rad_s_counter % 5 == 0) {
            rad_s_counter = 0;
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
            logger->log("Dead signal sent");
            e.super.sig = DEAD_BUTTON_LONGPRESS;
        } else {
            logger->log("God signal sent");
            e.super.sig = GOD_BUTTON_LONGPRESS;
        }
        QMsm_dispatch(the_health, &(e.super));
    }

}

void RadBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
    vibro->StartOrRestart(kBrr);
    logger->log("Pill id %i", pill_manager->ReadPill().id);
    healthQEvt e;
    switch (pill_manager->ReadPill().id) {
        case PILL_RESET: {
            e.super.sig = PILL_RESET_SIG;
            logger->log("Pill reset signal sent");
            QMsm_dispatch(the_health, &(e.super));
            break;
        }
        case PILL_HEAL: {
            e.super.sig = PILL_HEAL_SIG;
            logger->log("Pill heal signal sent");
            QMsm_dispatch(the_health, &(e.super));
            break;
        }
        case PILL_GOD: {
            e.super.sig = PILL_GOD_SIG;
            logger->log("Pill god signal sent");
            QMsm_dispatch(the_health, &(e.super));
            break;
        }
        default: {
            //do nothing, pill incorrect
            break;
        }
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

void RadBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
}

void RadBehavior::OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) {
    rx_table.AddPacket(packet);
}

void RadBehavior::StartTransmitForPath() {
    logger->log("Started path trasmittion");
    radio->SetBeaconPacket({path_id});
}

void RadBehavior::StopTransmitForPath() {
    logger->log("Stopped path trasmittion");
    radio->ClearBeaconPacket();
}

void RadBehavior::SetColor(Color color) {
    led->Stop();
    kRadSequence[0].color = color;
    led->StartOrRestart(kRadSequence);
}

void RadBehavior::MonsterVibro() {
    vibro->StartOrRestart(kBrrBrrBrr);
}

void RadBehavior::MakePillUsed() {
    logger->log("Pill cleared");
    pill_manager->WritePill({used_pill_id});
}

void RadBehavior::RadiationVibro() {
   vibro->StartOrRestart(kBrrBrr);
   beeper->StartOrRestart(kShortBeep);
}

void RadBehavior::Flash(Color color_new, Color color_old) {
    led->Stop();
    kRadFlashSequence[0].color = color_new;
    kRadFlashSequence[2].color = color_old;
    led->StartOrRestart(kRadFlashSequence);
};

void RadBehavior::LongVibro() {
    vibro->StartOrRestart(kLongBrr);
}

void RadBehavior::GodVibro() {
    vibro->StartOrRestart(kLongBrrForGod);
}
