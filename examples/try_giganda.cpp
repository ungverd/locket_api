//
// Created by juice on 02.08.2020.
//

#include "try_giganda.h"
#include "utility.h"

const RgbLedSequence StartLedSequence;
const RgbLedSequence ConnectLedSequence;
const RgbLedSequence AccelerateSequence;


void GigandaBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
    led->StartOrRestart(ConnectLedSequence);
}

void GigandaBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
    led->StartOrRestart(StartLedSequence);
}

void GigandaBehavior::OnStarted() {
    led->StartOrRestart(StartLedSequence);
    // мы считаем, что здесь вызвался OnDipSwitchChanged
}



void GigandaBehavior::OnButtonPressed(uint16_t button_index) {

    if (pill_manager) {
        uint32_t pill_id = pill_manager->ReadPill().id;
        switch (pill_id) {
            case 1: {
                if (LocketType == LocketEnum::REFRIGERATOR) {
                    led->StartOrRestart(AccelerateSequence);
                    pill_manager->WritePill({.id=0});
                }
                break;
            }
            default: {
                // some default commands here
            }
        }
    }
}

void GigandaBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {

    bool first = GetSwitchState(dip_value_mask, 7);
    bool second = GetSwitchState(dip_value_mask, 8);
    if (!first && second) {
        LocketType = LocketEnum::ACCELERATOR;
    } else if (first && !second) {
        LocketType = LocketEnum::REGENERATOR;
    } else if (first && second) {
        LocketType = LocketEnum::REFRIGERATOR;
    }
}
