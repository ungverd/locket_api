//
// Created by juice on 05.07.2021.
//

#ifndef LOCKET_API_SM_DATA_CPP_H
#define LOCKET_API_SM_DATA_CPP_H

#include "api/eeprom.h"

const unsigned int ABILUTY_SAVE_PAUSE = 60;
const unsigned int HEALTH_SAVE_PAUSE = 60;
const unsigned int DEFAULT_HP = 100;
const unsigned int GOD_PAUSE_M = 30;
const unsigned int ABILITY_PAUSE_M = 15;



class Health_Variables {
public:
    static Health_Variables Load(Eeprom* eeprom);
    void DecrementGodPause();
    unsigned int GetGodPause() const;
    void SetHealth(unsigned int hp);
    void ResetHealth();
    unsigned int GetHealth() const;
    void DecreaseHealth(unsigned int delta_hp);
    void ReSetGodPause();
    void ZeroGodPause();
    void IncrementCount();
    void ResetCount();
    unsigned int GetCount() const;

private:
    void SaveGodPause();
    void SaveCount();
    void SaveHealth();

    unsigned int god_pause;
    unsigned int count;
    unsigned int health;
    Eeprom* eeprom;
};

class Ability_Variables {
public:
    static Ability_Variables Load(Eeprom* eeprom);
    void DecrementAbilityPause();
    unsigned int GetAbilityPause() const;
    void SetAbility(unsigned int a);
    void ResetAbilityPause();
    void ResetCount();
    void IncrementCount();
    unsigned int GetCount() const;

private:
    void SaveAbilityPause();
    void SaveCount();
    void SaveAbility();

    unsigned int ability_pause;
    unsigned int count;
    unsigned int ability;
    Eeprom* eeprom;
};


struct EepromMap {
    Health_Variables health_vars;
    Ability_Variables  ability_vars;
    uint32_t ability_state;
    uint32_t health_state;
};

void SaveHealthState(Eeprom* eeprom, uint32_t State);


#endif //LOCKET_API_SM_DATA_CPP_H
