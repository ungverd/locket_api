#include "mariel_sm.h"
#include "Glue.h"
#include "health.h"

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
    s_counter++;
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
        e.super.sig = MONSTER_SIGNAl_SIG;
        QMsm_dispatch(the_health, &(e.super));
    } else if (rx_table.HasPacketWithId(rad_id)) {
        logger->log("Radiation");
        e.super.sig = RAD_RECEIVED_SIG;
        QMsm_dispatch(the_health, &(e.super));
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
    }
    QMsm_dispatch(the_health, &(e.super));
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

void RadBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
}

void RadBehavior::OnRadioPacketReceived(const IdOnlyState& packet) {
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
    kRadSequence[0].color = color;
    led->Stop();
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
}

void RadBehavior::Flash(Color color_new, Color color_old) {
    kRadFlashSequence[0].color = color_new;
    kRadFlashSequence[2].color = color_old;
    led->Stop();
    led->StartOrRestart(kRadFlashSequence);
};