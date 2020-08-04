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
    Color Color;
} __attribute__((packed));


#endif //LOCKET_API_SEQUENCES_H
