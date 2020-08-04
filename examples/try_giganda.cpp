//
// Created by juice on 02.08.2020.
//

#include "try_giganda.h"
#include "utility.h"

const RgbLedSequence StartLedSequence;
const RgbLedSequence ConnectLedSequence;

//stub player state sequences
const RgbLedSequence AccelerateSequence;
const RgbLedSequence RegenerateSequence;
const RgbLedSequence RefrigerateSequence;
const RgbLedSequence BombSequence;
const RgbLedSequence WrongSequence;

// stub master sequences
const RgbLedSequence EmptyMasterSequence;
const RgbLedSequence AccelerateMasterSequence;
const RgbLedSequence RegenerateMasterSequence;
const RgbLedSequence RefrigerateMasterSequence;
const RgbLedSequence LightMasterSequence;
const RgbLedSequence BombMasterSequence;


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
        if (LocketType == LocketEnum::MASTER) {
            master_pill_id = GetNextState(master_pill_id);
            pill_manager->WritePill({.id=master_pill_id});
        } else if (alive && state_timer==0) {
            uint32_t pill_id = pill_manager->ReadPill().id;
            PillEnum pill = IdToEnum(pill_id);
            switch (pill) {
                case PillEnum::ACCELERATE: {
                    if (LocketType == LocketEnum::ACCELERATOR) {
                        led->StartOrRestart(AccelerateSequence);
                        pill_manager->WritePill({.id=0});
                        state_timer = ACCELERATE_S;
                    }
                    break;
                }
                case PillEnum::REGENERATE: {
                    if (LocketType == LocketEnum::REGENERATOR) {
                        led->StartOrRestart(RegenerateSequence);
                        pill_manager->WritePill({.id=0});
                        state_timer = REGENERATE_S;
                    }
                    break;
                }
                case PillEnum::REFRIGERATE: {
                    if (LocketType == LocketEnum::REFRIGERATOR) {
                        led->StartOrRestart(RefrigerateSequence);
                        pill_manager->WritePill({.id=0});
                        state_timer = REFRIGERATE_S;
                    }
                    break;
                }
                case PillEnum::BOMB: {
                    led->StartOrRestart(BombSequence);
                    alive = false;
                    state_timer = BOMB_S;
                }
                default: {
                    led->StartOrRestart(WrongSequence);
                }
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
    bool master = GetSwitchState(dip_value_mask, 1);
    if (master) {
        LocketType = LocketEnum::MASTER;
    } else if (!first && second) {
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

uint8_t GigandaBehavior::GetNextState(uint8_t pill_id) {
    switch(pill_id) {
        case 0: {
            led->StartOrRestart(AccelerateMasterSequence);
            return 1;
        }
        case 1: {
            led->StartOrRestart(RegenerateMasterSequence);
            return 2;
        }
        case 2: {
            led->StartOrRestart(RefrigerateMasterSequence);
            return 3;
        }
        case 3: {
            led->StartOrRestart(LightMasterSequence);
            return 4;
        }
        case 4: {
            led->StartOrRestart(BombMasterSequence);
            return 5;
        }
        default: {
            led->StartOrRestart(EmptyMasterSequence);
            return 0;
        }
    }
}
