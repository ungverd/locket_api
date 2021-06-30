#include "mariel_sm.h"
#include "ability.h"
#include "health.h"
#include "Glue.h"

void RadBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    Ability_ctor(this, 0, 0);
    QMSM_INIT(the_ability, (QEvt *)0);
    Health_ctor(this, DEFAULT_HP, SIMPLE, 0);
    QMSM_INIT(the_ability, (QEvt *)0);

}

void RadBehavior::EverySecond() {
}

void RadBehavior::OnButtonPressed(uint16_t button_index) {
    abilityQEvt e;
    e.super.sig = LONG_PRESS_THIRD_SIG;
    // Printf("evtAbility: %d; %d\r", e.super.sig, e.value);
    QMSM_DISPATCH(the_ability, &(e.super));
}

void RadBehavior::Flash(unsigned int R, unsigned int G, unsigned int B, unsigned int Timeout) {
    logger->log("We are flashing furiously");
};
