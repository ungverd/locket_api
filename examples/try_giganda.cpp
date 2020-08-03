//
// Created by juice on 02.08.2020.
//

#include "try_giganda.h"
#include "utility.h"

const RgbLedSequence StartLedSequence;
const RgbLedSequence ConnectLedSequence;
const RgbLedSequence AccelerateSequence;
const RgbLedSequence RegenerateSequence;
const RgbLedSequence RefrigerateSequence;
const RgbLedSequence BombSequence;
const RgbLedSequence WrongSequence;


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

    if (pill_manager && alive && state_timer==0) {
        uint32_t pill_id = pill_manager->ReadPill().id;
        PillEnum pill = IdToEnum(pill_id);
        switch (pill) {
            case PillEnum::ACCELERATE: {
                if (LocketType == LocketEnum::ACCELERATOR) {
                    led->StartOrRestart(AccelerateSequence);
                    pill_manager->WritePill({.id=0});
                    state_timer = ACCELERATE_MS;
                }
                break;
            }
            case PillEnum::REGENERATE: {
                if (LocketType == LocketEnum::REGENERATOR) {
                    led->StartOrRestart(RegenerateSequence);
                    pill_manager->WritePill({.id=0});
                    state_timer = REFRIGERATE_MS;
                }
                break;
            }
            case PillEnum::REFRIGERATE: {
                if (LocketType == LocketEnum::REFRIGERATOR) {
                    led->StartOrRestart(RefrigerateSequence);
                    pill_manager->WritePill({.id=0});
                    state_timer = REFRIGERATE_MS;
                }
                break;
            }
            case PillEnum::BOMB:  {
                led->StartOrRestart(BombSequence);
                alive = false;
                state_timer = BOMB_MS;
            }
            default: {
                led->StartOrRestart(WrongSequence);
            }
        }
    }
}

void GigandaBehavior::EverySecond() {

    if (state_timer > 0) {
        state_timer--;
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

PillEnum GigandaBehavior::IdToEnum(uint8_t id) {

    switch (id) {
        case 0:
            return PillEnum::EMPTY;
        case 1:
            return PillEnum::ACCELERATE;
        case 2:
            return PillEnum::REGENERATE;
        case 3:
            return  PillEnum::REFRIGERATE;
        case 4:
            return PillEnum::LIGHT;
        case 5:
            return PillEnum::BOMB;
    }
    return PillEnum::EMPTY;
}


