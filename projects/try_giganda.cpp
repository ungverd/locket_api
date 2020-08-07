//
// Created by juice on 02.08.2020.
//

#include "try_giganda.h"
#include "utility.h"


const LedRGBChunk StartOnceLedSequence[] = {
        {ChunkType::kSetup, 0, kLightGreen},
        {ChunkType::kWait, 1000},
        {ChunkType::kEnd}
};
const LedRGBChunk ConnectOnceLedSequence[] = {
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};

const LedRGBChunk OffSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 1000},
        {ChunkType::kEnd}
};

//stub player state sequences
const LedRGBChunk AccelerateSequence[] = {
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kWait, 1000},
        {ChunkType::kEnd}
};
const LedRGBChunk RegenerateSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 1000},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};

const LedRGBChunk RefrigerateSequence[] = {
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd},
};

const LedRGBChunk BombSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk WrongOnceSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};

// stub master sequences
const LedRGBChunk EmptyMasterSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk AccelerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk RegenerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk RefrigerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk LightMasterSequence[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};
const LedRGBChunk BombMasterSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 500},
        {ChunkType::kEnd}
};

void GigandaBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
    led->StartOrRestart(ConnectOnceLedSequence);
    led->StartOrRestart(OffSequence);
}

void GigandaBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
    led->StartOrRestart(StartOnceLedSequence);
    led->StartOrRestart(OffSequence);
}

void GigandaBehavior::OnStarted() {
    led->StartOrRestart(StartOnceLedSequence);
    led->StartOrRestart(OffSequence);
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
                    led->StartOrRestart(WrongOnceSequence);
                    led->StartOrRestart(OffSequence);
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
