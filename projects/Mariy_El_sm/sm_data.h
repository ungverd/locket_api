//
// Created by juice on 05.07.2021.
//

#ifndef LOCKET_API_SM_DATA_CPP_H
#define LOCKET_API_SM_DATA_CPP_H

#include "api/eeprom.h"
#include "color.h"

const unsigned int HEALTH_SAVE_PAUSE = 60;
const unsigned int DEFAULT_HP = 330;
const unsigned int GOD_PAUSE_M = 30;



class Health_Variables {
public:
    static Health_Variables Load(Eeprom* eeprom);
    void DecrementGodPause();
    [[nodiscard]] unsigned int GetGodPause() const;
    void ResetHealth();
    [[nodiscard]] unsigned int GetHealth() const;
    void DecreaseHealth(unsigned int delta_hp);
    void ReSetGodPause();
    void ZeroGodPause();
    void IncrementCount();
    void ResetCount();
    [[nodiscard]] Color GetHealthColor() const;
    [[nodiscard]] unsigned int GetCount() const;

private:
    void SaveGodPause();
    void SaveCount();
    void SaveHealth();

    unsigned int god_pause;
    unsigned int count;
    unsigned int health;
    Eeprom* eeprom;
};

struct EepromMap {
    Health_Variables health_vars;
    //used to save health sm state to EEPROM
    uint32_t health_state;
};

void SaveHealthState(Eeprom* eeprom, uint32_t State);


#endif //LOCKET_API_SM_DATA_CPP_H
