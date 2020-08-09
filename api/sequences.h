#ifndef LOCKET_API_SEQUENCES_H
#define LOCKET_API_SEQUENCES_H
#include <cstdint>
#include "color.h"

// Sequences to control RGB LED, vibro, beeper.
// See example sequences in the end of file.
// Sequence is a series (represented as C-style array) of commands (aka chunks) for indication peripheral
// (e.g. RGB led or beeper) which can be executed independently and in parallel with main program flow.
// There are following types of commands:
enum class ChunkType {
// Does actual indication: set LED to specific color or turns on vibro.
    kSetup,
// Waits for given (by `time_ms`, see below) number of milliseconds.
// Important: this wait happens in parallel with the main program flow.
// Running the sequence doesn't block main program execution, even if sequence contains kWait's.
    kWait,
// Execution goes to the specified (by `chunk_to_jump_to`, see below) chunk of the sequence.
// Can be used to create "infinite" sequences.
    kGoto,
// Ends the sequence execution. Last command of every sequence must be either kEnd (for finite sequences) or kGoto
// (for finite ones). Sequence is a C-style array (so we don't know it's length), so we need a marker of array end.
// kEnd is that marker.
    kEnd,
// Repeats the execution of all previous chunks specified (by `repeat_count`, see below) number of times, than goes
// to the next chunk. Can be used to create long (but finite!) repetitive sequences:
// <repetitive part>, kRepeat, kEnd.
    kRepeat
};

struct BaseChunk {
    ChunkType type;
// Every chunk (except ones of type kEnd) has an additional information affecting the behavior in addition to type.
// They are all packed together, essentially, value/volume/time_ms/chunk_to_jump_to/repeat_count are just different
// names of the same variable/memory location.
// So chunk can be initialized like this:
//   const BaseChunk c = {ChunkType::kSetup, 10}.
// Now c.value == c.volume == c.time_ms == c.chunk_to_jump_to == c.repeat_count == 10.
    union {
        uint32_t value; // Used by BeepChunk
        uint32_t volume; // Used by BeepChunk
        uint32_t time_ms; // Used when type == ChunkType::kWait
        uint32_t chunk_to_jump_to; // Used when type == ChunkType::kGoto
        int32_t repeat_count; // Used when type == ChunkType::kRepeat
    };
} __attribute__((packed));

// Extended version of BaseChunk having additional data (used when chunk type is ChunkType::kSetup) to control RGB LED.
struct LedRGBChunk: BaseChunk {
    Color color;
} __attribute__((packed));

// Extended version of BaseChunk having additional data (used when chunk type is ChunkType::kSetup) to control beeper.
struct BeepChunk: BaseChunk {
    // value == beeping volume
    uint16_t freq_hz;
} __attribute__((packed));

// Not-really extended version of BaseChunk used to control vibro. Declared as a separate class for consistency.
struct VibroChunk: BaseChunk {
} __attribute__((packed));

//
// Standard, non-project specific sequences. Feel free to use them as examples.
// If you need to modify one of those for your project - copy it to your behavior.cpp file, rename and modify the copy.
//

// Short red-green-blue-off sequence. Typically used as an indication of successful startup
// (so use it in your OnStarted).
const LedRGBChunk kStartSequence[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};

// 3 red blinks with a small delay between them. Typically used as an error indication.
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

// Short single beep.
const BeepChunk kShortBeep[] = {
        {ChunkType::kSetup, 1, 1975},
        {ChunkType::kWait, 54},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};

constexpr const uint32_t kVibroVolume = 100; // Can be 1 to 100
constexpr const uint32_t kVibroShortDelayMs = 99;
constexpr const uint32_t kVibroRepeatPeriod = 1008;

// Single vibration
const VibroChunk kBrr[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, kVibroRepeatPeriod},
        {ChunkType::kEnd}
};

// Double vibration
const VibroChunk kBrrBrr[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, kVibroRepeatPeriod},
        {ChunkType::kEnd}
};

// Triple vibration
const VibroChunk kBrrBrrBrr[] = {
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, kVibroVolume},
        {ChunkType::kWait, kVibroShortDelayMs},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, kVibroRepeatPeriod},
        {ChunkType::kEnd}
};


#endif //LOCKET_API_SEQUENCES_H
