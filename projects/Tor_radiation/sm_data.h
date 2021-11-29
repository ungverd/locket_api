//
// Created by juice on 05.07.2021.
//

#ifndef LOCKET_API_SM_DATA_CPP_H
#define LOCKET_API_SM_DATA_CPP_H

#include "api/eeprom.h"
#include "color.h"

const unsigned int SAVE_COUNT_PAUSE=60;

class Health_Variables {
public:
    static Health_Variables Load(Eeprom* eeprom);
    void DecrementCount();
    void SetCount(unsigned int new_count);
    unsigned int GetCount();
    bool IsPositive();

private:
    void SaveCount();

    unsigned int count;
    Eeprom* eeprom;
};

struct EepromMap {
    Health_Variables health_vars;
    //used to save health sm state to EEPROM
    uint32_t health_state;
};

void SaveHealthState(Eeprom* eeprom, uint32_t State);


#endif //LOCKET_API_SM_DATA_CPP_H
