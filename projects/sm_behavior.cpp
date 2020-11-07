#include "sm_behavior.h"

void StateMachineBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void StateMachineBehavior::EverySecond() {
}