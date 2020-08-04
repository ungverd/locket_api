#include "basic_behavior.h"
#include "utility.h"

void BasicBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(RgbLedSequence());
}

void BasicBehavior::EverySecond() {
    ++seconds_counter;
    if (seconds_counter % 30 == 0) {
        logger->log("30 seconds passed");
    }
}

void BasicBehavior::OnPillConnected(PillManager<IdOnlyState>* manager) {
    pill_manager = manager;
    logger->log("Read value %d from pill", pill_manager->ReadPill().id);
}

void BasicBehavior::OnPillDisconnected() {
    logger->log("Pill was disconnected");
    pill_manager = nullptr;
}

void BasicBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {
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
