//
// Created by juice on 05.07.2021.
//

#include "sm_data.h"
#include <cstddef>


//start of health variables structure functions
Health_Variables Health_Variables::Load(Eeprom* eeprom) {
    Health_Variables result{};
    result.eeprom = eeprom;
    result.count = eeprom->Read<unsigned int>(offsetof(EepromMap, health_vars)
            + offsetof(Health_Variables, count));
    return result;
}


void Health_Variables::IncrementCount() {
    ++count;
    // Count changes every second, only persist it occasionally to prevent
    // too frequent eeprom writes.
    if (count % SAVE_COUNT_PAUSE == 0) {
        SaveCount();
    }
}

void Health_Variables::ResetCount() {
    count = 0;
    SaveCount();
}

void Health_Variables::SaveCount() {
    eeprom->Write(offsetof(Health_Variables, count) + offsetof(EepromMap, health_vars),count);
}

//end of Health_Variables functions


void SaveHealthState(Eeprom* eeprom, uint32_t State) {
    eeprom->Write(offsetof(EepromMap, health_state), State);
}
