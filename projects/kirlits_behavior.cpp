//
// Created by juice on 24.08.2020.
//
#include "kirlits.h"
#include "utility.h"


const int FlashDur = 500;

const LedRGBChunk kDoubleYellow[] = {
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kYellow},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const LedRGBChunk kDoubleRed[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const LedRGBChunk kDoubleBlue[] = {
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const LedRGBChunk kDoubleWhite[] = {
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kWhite},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const LedRGBChunk kRedAndBlue[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

void KirlitsBehavior::OnStarted() {
    logger->log("Started execution!");
    led->StartOrRestart(kStartSequence);
}

void KirlitsBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {

    bool first = GetSwitchState(dip_value_mask, 6);
    bool second = GetSwitchState(dip_value_mask, 7);
    bool third = GetSwitchState(dip_value_mask, 8);
    uint8_t CurrentType = first*4 + second*2 + third;
    LocketType = IdToEnum(CurrentType);
}

void KirlitsBehavior::OnRadioPacketReceived(const IdAndTypeState& packet) {
    rx_table.AddPacket(packet);
}


LocketEnum KirlitsBehavior::IdToEnum(uint8_t id) {

    switch (id) {
        case 0:
            return LocketEnum::YELLOW;
        case 1:
            return LocketEnum::WHITE;
        case 2:
            return  LocketEnum::BLUE;
        case 3:
            return LocketEnum::RED;
        case 4:
            return LocketEnum::BLUERED;
        case 5:
            return LocketEnum::SILENT;
        default:
            return LocketEnum::YELLOW;
    }
}