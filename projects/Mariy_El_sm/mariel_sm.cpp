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

void RadBehavior::Flash(unsigned int R, unsigned int G, unsigned int B, unsigned int Timeout) {
    logger->log("We are flashing furiously");
};
