//
// Created by juice on 02.08.2020.
//

#include "giganda_behavior.h"
#include "utility.h"


const LedRGBChunk StartOnceLedSequence[] = {
        {ChunkType::kSetup, 0, kLightGreen},
        {ChunkType::kWait, 1000},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};
const LedRGBChunk ConnectOnceLedSequence[] = {
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

const LedRGBChunk OffSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

//stub player state sequences
const LedRGBChunk AccelerateSequence[] = {
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kWait, 1000*ACCELERATE_S},
        {ChunkType::kEnd}

};
const LedRGBChunk RegenerateSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kGoto, 0},
};

const LedRGBChunk RefrigerateSequence[] = {
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kGoto, 0}
};

const LedRGBChunk BombSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 330},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 330},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 330},
        {ChunkType::kGoto, 0}
};
const LedRGBChunk WrongOnceSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

// stub master sequences
const LedRGBChunk EmptyMasterSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kEnd}
};
const LedRGBChunk AccelerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kEnd}
};
const LedRGBChunk RegenerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kEnd}
};
const LedRGBChunk RefrigerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kEnd}
};
const LedRGBChunk LightMasterSequence[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kEnd}
};
const LedRGBChunk BombMasterSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kEnd}
};

const VibroChunk kBrrLong[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, 1000},
        {ChunkType::kEnd}
};

void GigandaBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
    led->StartOrRestart(ConnectOnceLedSequence);
    vibro->StartOrRestart(kBrr);
}

void GigandaBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
    led->StartOrRestart(StartOnceLedSequence);
}

void GigandaBehavior::OnStarted() {
    led->StartOrRestart(StartOnceLedSequence);
    vibro->StartOrRestart(kBrrBrrBrr);
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
                        vibro->StartOrRestart(kBrr);
                        pill_manager->WritePill({.id=0});
                        state_timer = ACCELERATE_S;
                    }
                    break;
                }
                case PillEnum::REGENERATE: {
                    if (LocketType == LocketEnum::REGENERATOR) {
                        led->StartOrRestart(RegenerateSequence);
                        vibro->StartOrRestart(kBrrBrr);
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
                    vibro->StartOrRestart(kBrrLong);
                    alive = false;
                    state_timer = BOMB_S;
                }
                default: {
                    led->StartOrRestart(WrongOnceSequence);
                }
            }
        } else if (LocketType == LocketEnum::MASTER) {
            master_pill_id = GetNextState(master_pill_id);
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
