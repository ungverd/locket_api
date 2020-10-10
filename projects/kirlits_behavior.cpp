//
// Created by juice on 24.08.2020.
//
#include "kirlits.h"
#include "utility.h"


const int FlashDur = 500;
const int MaxLocketNum = 16;
const int MaxChunksForLocket = 9;
const int MaxChunkNum = MaxChunksForLocket*MaxLocketNum;
const int TypesNum = 6;

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
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const LedRGBChunk* Sequences[] = {kDoubleYellow, kDoubleWhite, kDoubleBlue, kDoubleRed, kRedAndBlue};

void KirlitsBehavior::OnStarted() {
    logger->log("Started execution!");
    sequence.reserve(MaxChunkNum);
    led->StartOrRestart(kStartSequence);
    radio->SetBeaconPacket({LocketId, EnumToType()});
}

void KirlitsBehavior::OnDipSwitchChanged(uint16_t dip_value_mask) {

    bool FirstType = GetSwitchState(dip_value_mask, 6);
    bool SecondType= GetSwitchState(dip_value_mask, 7);
    bool ThirdType = GetSwitchState(dip_value_mask, 8);
    bool FirstId = GetSwitchState(dip_value_mask, 1);
    bool SecondId = GetSwitchState(dip_value_mask, 2);
    bool ThirdId= GetSwitchState(dip_value_mask, 3);
    uint8_t CurrentType = FirstType*4 + SecondType*2 + ThirdType;
    LocketId = FirstId*4 + SecondId*2 + ThirdId;
    LocketType = TypeToEnum(CurrentType);
    radio->ClearBeaconPacket();
    radio->SetBeaconPacket({LocketId, EnumToType()});
    logger->log("Radio Started %i % i", LocketId, EnumToType());
}

void KirlitsBehavior::EverySecond() {
    ++seconds_counter;
    if (seconds_counter % 60 == 0) {
        int LocketsNear[TypesNum] = {};
        logger->log("MinutePassed!");
        for (int i = 0; i < rx_table.Raw().size(); i++) {
            LocketsNear[rx_table.Raw()[i].locket_type] += 1;
        }
        for (int i = 0; i < TypesNum-1; i++) {
            for (int j = 0; j < LocketsNear[i]; j++) {
                for (int k = 0; k < sizeof(*Sequences[i])/sizeof(LedRGBChunk); k++) {
                    sequence.push_back(Sequences[i][k]);
                }
            }
        }
        led->Stop();
        rx_table.Clear();
        logger->log("LedsStarted!");
        led->StartOrRestart(sequence.data());
        if (LocketsNear[TypesNum-1] != 0) {
            radio->ClearBeaconPacket();
            logger->log("Radio Off");
        } else {
            radio->SetBeaconPacket({LocketId, EnumToType()});
            logger->log("Radio Started %i % i", LocketId, EnumToType());
        }
        seconds_counter = 0;
    }
}

void KirlitsBehavior::OnRadioPacketReceived(const IdAndTypeState& packet) {
    rx_table.AddPacket(packet);
}


LocketEnum KirlitsBehavior::TypeToEnum(uint8_t id) {

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

uint8_t KirlitsBehavior::EnumToType() {

    switch (LocketType) {
        case LocketEnum::YELLOW:
            return 0;
        case LocketEnum::WHITE:
            return 1;
        case LocketEnum::BLUE:
            return 2;
        case LocketEnum::RED:
            return 3;
        case LocketEnum::BLUERED:
            return 4;
        case LocketEnum::SILENT:
            return 5;
        default:
            return 0;
    }
}
