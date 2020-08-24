//
// Created by juice on 24.08.2020.
//
#include "kirlits.h"

const int FlashDur = 500;

// 3 red blinks with a small delay between them. Typically used as an error indication.
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