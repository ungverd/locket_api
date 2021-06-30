#include "sm_behavior.h"
#include "ability.h"
#include "Glue.h"

void StateMachineBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
    Ability_ctor(this, MUTANT_READY, 0);
    QMSM_INIT(the_ability, (QEvt *)0);

}

void StateMachineBehavior::EverySecond() {
}

void StateMachineBehavior::OnButtonPressed(uint16_t button_index) {
    abilityQEvt e;
    // e.super.sig = LONG_PRESS_THIRD_SIG;
    // Printf("evtAbility: %d; %d\r", e.super.sig, e.value);
    QMSM_DISPATCH(the_ability, &(e.super));
}

void StateMachineBehavior::Flash(unsigned int R, unsigned int G, unsigned int B, unsigned int Timeout) {
    logger->log("We are flashing furiously");
};
