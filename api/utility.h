#ifndef LOCKET_API_UTILITY_H
#define LOCKET_API_UTILITY_H

#include <cstdint>
#include "radio.h"

// Given the DIP switch state as a bit mask (see Behavior::OnDipSwitchChanged which gets it as an argument), and
// position (1-based, i.e. 1..8) - returns if corresponding switch is set to ON.
bool GetSwitchState(uint16_t dip_value_mask, uint8_t position);

//function to convert uint8_t id to RadioPowerLevel type for convinient setting radio power level by DIPSwitches
RadioPowerLevel IdToRadioEnum(uint8_t id);

#endif //LOCKET_API_UTILITY_H
