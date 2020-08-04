#ifndef LOCKET_API_SEQUENCES_H
#define LOCKET_API_SEQUENCES_H

#include <cstdint>
#include "color.h"

enum class ChunkType {kSetup, kWait, kGoto, kEnd, kRepeat};

struct BaseChunk {
    ChunkType type;
    union {
        uint32_t value;
        uint32_t volume;
        uint32_t time_ms;
        uint32_t chunk_to_jump_to;
        int32_t repeat_count;
    };
} __attribute__((packed));

struct LedRGBChunk: BaseChunk {
    Color color;
} __attribute__((packed));

struct BeepChunk: BaseChunk {
    // Value == Volume
    uint16_t freq_hz;
} __attribute__((packed));

const LedRGBChunk kStartSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, {0, 4, 0}},
        {ChunkType::kEnd},
};

const LedRGBChunk kFailureSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

#endif //LOCKET_API_SEQUENCES_H
