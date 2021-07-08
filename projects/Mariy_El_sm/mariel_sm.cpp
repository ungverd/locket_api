#include "mariel_sm.h"
#include "Glue.h"
#include "health.h"

void RadBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    Health_ctor(this, SIMPLE, this->eeprom, this->logger);
    QMSM_INIT(the_health, (QEvt *)nullptr);

}

void RadBehavior::EverySecond() {
}

void RadBehavior::OnButtonPressed(uint16_t button_index, bool long_press) {
    healthQEvt e;
    e.super.sig = LONG_PRESS_THIRD_SIG;
    // Printf("evtAbility: %d; %d\r", e.super.sig, e.value);
    QMSM_DISPATCH(the_health, &(e.super));
}

void RadBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
}

void RadBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
}

void RadBehavior::StartTransmitForPath() {
    radio->SetBeaconPacket({path_id});
}

void RadBehavior::SetColor(Color color) {
    const LedRGBChunk kRadSequence[] = {
            {ChunkType::kSetup, 0, color},
            {ChunkType::kWait, 100},
            {ChunkType::kGoto, 0},
    };
    led->StartOrRestart(kRadSequence);
}

void RadBehavior::MonsterVibro() {
    vibro->StartOrRestart(kBrrBrrBrr);
}

void RadBehavior::MakePillUsed() {
    pill_manager->WritePill({used_pill_id});
}

void RadBehavior::RadiationVibro() {
   vibro->StartOrRestart(kBrr);
}

void RadBehavior::Flash(Color color_new, Color color_old) {
    const LedRGBChunk kRadFlashSequence[] = {
            {ChunkType::kSetup, 0, color_new},
            {ChunkType::kWait, 500},
            {ChunkType::kSetup, 0, color_old},
            {ChunkType::kWait, 100},
            {ChunkType::kGoto, 2},
    };
    led->StartOrRestart(kRadFlashSequence);
};