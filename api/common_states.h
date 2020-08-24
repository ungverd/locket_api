#ifndef LOCKET_API_COMMON_STATES_H
#define LOCKET_API_COMMON_STATES_H

#include <cstdint>

// Use this if state doesn't matter or not used at all.
struct EmptyState {};

// Use this when you only need to store 32-bit identifier in the state.
struct IdOnlyState {
    uint32_t id;
};


// Type for transmitting id and type
struct IdAndTypeState {
    uint32_t id;
    uint8_t state;
};

#endif //LOCKET_API_COMMON_STATES_H
