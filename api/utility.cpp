#include "utility.h"

bool GetSwitchState(uint16_t dip_value_mask, uint8_t position) {
    return dip_value_mask & (1 << (position - 1));
}
