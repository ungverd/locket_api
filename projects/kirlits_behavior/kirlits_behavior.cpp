//
// Created by juice on 24.08.2020.
//
#include "kirlits_behavior.h"
#include "utility.h"


const int FlashDur = 500;
const int MaxLocketNum = 16;
const int MaxChunksForLocket = 9;
const int MaxChunkNum = MaxChunksForLocket*MaxLocketNum;
const int TypesNum = 6;

const std::vector<LedRGBChunk> kDoubleYellow = {
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

const std::vector<LedRGBChunk> kDoubleRed = {
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

const std::vector<LedRGBChunk> kDoubleBlue = {
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

const std::vector<LedRGBChunk> kDoubleWhite = {
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

const std::vector<LedRGBChunk> kRedAndBlue = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, FlashDur},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait,  FlashDur},
        {ChunkType::kEnd}
};

const std::vector<const std::vector<LedRGBChunk>*> Sequences = {
        &kDoubleYellow,
        &kDoubleWhite,
        &kDoubleBlue,
        &kDoubleRed,
        &kRedAndBlue
};

void KirlitsBehavior::OnStarted() {
    logger->log("Started execution!");
    sequence.reserve(MaxChunkNum);
    radio->SetBeaconPacket({LocketId, EnumToType(LocketType)});
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
    radio->SetBeaconPacket({LocketId, CurrentType});
    logger->log("Radio Started %i % i", LocketId, CurrentType);
    if (CurrentType < Sequences.size()) {
        led->StartOrRestart(Sequences[EnumToType(LocketType)]->data());
    } else {
        led->StartOrRestart(kStartSequence);
    }
}

void KirlitsBehavior::EverySecond() {
    ++seconds_counter;
    if (seconds_counter % 60 == 0) {
        logger->log("MinutePassed!");
        CreateSequence(rx_table.Raw(), sequence);
        led->Stop();
        logger->log("LedsStarted!");
        led->StartOrRestart(sequence.data());
        if (IfSilentNear(rx_table.Raw())) {
            radio->ClearBeaconPacket();
            logger->log("Radio Off");
        } else {
            radio->SetBeaconPacket({LocketId, EnumToType(LocketType)});
            logger->log("Radio Started %i % i", LocketId, EnumToType(LocketType));
        }
        rx_table.Clear();
        seconds_counter = 0;
    }
}

void KirlitsBehavior::OnRadioPacketReceived(const IdAndTypeState& packet, int8_t rssi) {
    rx_table.AddPacket(packet);
}


LocketEnum TypeToEnum(uint8_t id) {

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

uint8_t EnumToType(LocketEnum LocketType) {

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

void CreateSequence(const std::vector<IdAndTypeState>& rx_table, std::vector<LedRGBChunk>& chunk_sequence) {
    chunk_sequence.clear();
    int LocketsNear[TypesNum] = {};
    for (const IdAndTypeState &packet: rx_table) {
        LocketsNear[packet.locket_type] += 1;
    }

    for (int i = 0; i < TypesNum - 1; i++) {
        for (int j = 0; j < LocketsNear[i]; j++) {
            for (unsigned int k = 0; k < Sequences[i]->size() - 1; k++) {
                chunk_sequence.push_back((*Sequences[i])[k]);
            }
        }
    }
    chunk_sequence.push_back({ChunkType::kEnd});
}

bool IfSilentNear(const std::vector<IdAndTypeState>& rx_table) {
    for (const IdAndTypeState &packet: rx_table) {
        if (packet.locket_type == EnumToType(LocketEnum::SILENT)) {
            return true;
        }
    }
    return false;
}