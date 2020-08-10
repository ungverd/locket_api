#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
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
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kEnd}
};
const LedRGBChunk AccelerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kEnd}
};
const LedRGBChunk RegenerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kEnd}
};
const LedRGBChunk RefrigerateMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kMagenta},
        {ChunkType::kEnd}
};
const LedRGBChunk LightMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kEnd}
};
const LedRGBChunk BombMasterSequence[] = {
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 500},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kEnd}
};

const LedRGBChunk* MasterSequences[] = {EmptyMasterSequence, AccelerateMasterSequence,
                                        RegenerateMasterSequence, RefrigerateMasterSequence,
                                        LightMasterSequence, BombMasterSequence};

const VibroChunk kBrrLong[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, 1000*BOMB_S},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};

void GigandaBehavior::OnPillConnected(PillManager<IdOnlyState> *manager) {
    pill_manager = manager;
    if (LocketType != LocketEnum::MASTER) {
        led->StartOrRestart(ConnectOnceLedSequence);
        vibro->StartOrRestart(kBrr);
    }
}

void GigandaBehavior::OnPillDisconnected() {
    pill_manager = nullptr;
}

void GigandaBehavior::OnStarted() {
    // мы считаем, что здесь вызвался OnDipSwitchChanged
    if (LocketType == LocketEnum::MASTER) {
        led->StartOrRestart(EmptyMasterSequence);
    } else {
        led->StartOrRestart(StartOnceLedSequence);
        vibro->StartOrRestart(kBrrBrrBrr);
    }
}

void GigandaBehavior::OnButtonPressed(uint16_t button_index) {

    if (LocketType == LocketEnum::MASTER) {
        master_pill_id = GetNextState(master_pill_id);
    } else if (pill_manager) {
        if (LocketType == LocketEnum::MASTER) {
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

    if (state_timer > 1) {
        state_timer--;
    } else if (state_timer == 1) {
        led->StartOrRestart(OffSequence);
        state_timer = 0;
    }
    if (pill_manager && LocketType == LocketEnum::MASTER) {
        pill_manager->WritePill({.id=master_pill_id});
        led->StartOrRestart(MasterSequences[master_pill_id]);
    }
}

void GigandaBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {

    bool first = GetSwitchState(dip_value_mask, 7);
    bool second = GetSwitchState(dip_value_mask, 8);
    bool master = GetSwitchState(dip_value_mask, 1);
    if (master) {
        LocketType = LocketEnum::MASTER;
        led->StartOrRestart(EmptyMasterSequence);
    } else if (!first && second) {
        LocketType = LocketEnum::ACCELERATOR;
    } else if (first && !second) {
        LocketType = LocketEnum::REGENERATOR;
    } else if (first && second) {
        LocketType = LocketEnum::REFRIGERATOR;
    }
    if (LocketType != LocketEnum::MASTER) {
        led->StartOrRestart(StartOnceLedSequence);
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
        default:
            return PillEnum::EMPTY;
    }
}

uint8_t GigandaBehavior::GetNextState(uint8_t pill_id) {
    if (pill_id == 5) {
        led->StartOrRestart(MasterSequences[0]);
        return 0;
    }
    led->StartOrRestart(MasterSequences[pill_id + 1]);
    return pill_id + 1;
}

#pragma clang diagnostic pop