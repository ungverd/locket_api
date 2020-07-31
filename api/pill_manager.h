#ifndef LOCKET_API_PILL_MANAGER_H
#define LOCKET_API_PILL_MANAGER_H

#include "common_states.h"

template <typename PillState = IdOnlyState>
class PillManager {
public:
    virtual PillState ReadPill() = 0;
    virtual void WritePill(const PillState& state) = 0;
};

#endif //LOCKET_API_PILL_MANAGER_H
