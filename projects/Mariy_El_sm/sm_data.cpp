//
// Created by juice on 05.07.2021.
//

#include "sm_data.h"
#include <cstddef>


//start of health variables structure functions
Health_Variables Health_Variables::Load(Eeprom* eeprom) {
    Health_Variables result{};
    result.eeprom = eeprom;
    result.god_pause = eeprom->Read<unsigned int>(offsetof(EepromMap, health_vars)
            + offsetof(Health_Variables, god_pause));
    result.count = eeprom->Read<unsigned int>(offsetof(EepromMap, health_vars)
            + offsetof(Health_Variables, count));
    result.health = eeprom->Read<unsigned int>(offsetof(EepromMap, health_vars)
            + offsetof(Health_Variables, health));
    return result;
}

void Health_Variables::ResetHealth() {
    health = DEFAULT_HP;
    SaveHealth();
}

void Health_Variables::DecrementGodPause() {
    --god_pause;
    SaveGodPause();
}

unsigned int Health_Variables::GetGodPause() const {
    return god_pause;
}

unsigned int Health_Variables::GetHealth() const {
    return health;
}

void Health_Variables::DecreaseHealth(unsigned int delta_hp) {
    if (delta_hp <= health) {
        health -= delta_hp;
    } else {
        health = 0;
    }
    SaveHealth();
}


void Health_Variables::ReSetGodPause() {
    god_pause = GOD_PAUSE_M;
    SaveGodPause();
}

void Health_Variables::ZeroGodPause() {
    god_pause = 0;
    SaveGodPause();
}

void Health_Variables::IncrementCount() {
    ++count;
    // Count changes every second, only persist it occasionally to prevent
    // too frequent eeprom writes.
    if (count % HEALTH_SAVE_PAUSE == 0) {
        SaveCount();
    }
}

void Health_Variables::ResetCount() {
    count = 0;
    SaveCount();
}

unsigned int Health_Variables::GetCount() const {
    return count;
}

Color Health_Variables::GetHealthColor() const{
    uint8_t red =  255 - 255*health/DEFAULT_HP;
    uint8_t green = 255*health/DEFAULT_HP;
    Color CurrentHealthColor = Color(red, green,0);
    return CurrentHealthColor;
}

void Health_Variables::SaveGodPause() {
    eeprom->Write(offsetof(Health_Variables, god_pause) + offsetof(EepromMap, health_vars), god_pause);
}
void Health_Variables::SaveCount() {
    eeprom->Write(offsetof(Health_Variables, count) + offsetof(EepromMap, health_vars),count);
}
void Health_Variables::SaveHealth() {
    eeprom->Write(offsetof(Health_Variables, health) + offsetof(EepromMap, health_vars), health);
}
//end of Health_Variables functions


void SaveHealthState(Eeprom* eeprom, uint32_t State) {
    eeprom->Write(offsetof(EepromMap, health_state), State);
}
