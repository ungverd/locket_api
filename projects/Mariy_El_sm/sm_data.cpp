//
// Created by juice on 05.07.2021.
//

#include "sm_data.h"
#include "ability.h"
#include "health.h"

//start of health variables structure functions
Health_Variables Health_Variables::Load(Eeprom* eeprom) {
    Health_Variables result{};
    result.eeprom = eeprom;
    result.god_pause = eeprom->Read<unsigned int>(offsetof(EepromMap, vars1)
            + offsetof(Health_Variables, god_pause));
    result.count = eeprom->Read<unsigned int>(offsetof(EepromMap, vars1)
            + offsetof(Health_Variables, count));
    result.health = eeprom->Read<unsigned int>(offsetof(EepromMap, vars1)
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

void Health_Variables::SetHealth(unsigned int hp) {
    health = hp;
    SaveHealth();
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

void Health_Variables::SaveGodPause() {
    eeprom->Write(god_pause, offsetof(Health_Variables, god_pause) + offsetof(EepromMap, vars1));
}
void Health_Variables::SaveCount() {
    eeprom->Write(count, offsetof(Health_Variables, count) + offsetof(EepromMap, vars1));
}
void Health_Variables::SaveHealth() {
    eeprom->Write(health, offsetof(Health_Variables, health) + offsetof(EepromMap, vars1));
}
//end of Health_Variables functions

//start of Ability_Variables functions
Ability_Variables Ability_Variables::Load(Eeprom* eeprom) {
    Ability_Variables result{};
    result.eeprom = eeprom;
    result.ability_pause = eeprom->Read<unsigned int>(offsetof(EepromMap, vars2)
            + offsetof(Ability_Variables, ability_pause));
    result.count = eeprom->Read<unsigned int>(offsetof(EepromMap, vars2)
            + offsetof(Ability_Variables, count));
    result.ability = eeprom->Read<unsigned int>(offsetof(EepromMap, vars2)
            + offsetof(Ability_Variables, ability));
    return result;
}

void Ability_Variables::DecrementAbilityPause() {
    --ability_pause;
    SaveAbilityPause();
}

unsigned int Ability_Variables::GetAbilityPause() const {
    return ability_pause;
}

void Ability_Variables::SetAbility(unsigned int a) {
    ability = a;
    SaveAbility();
}

void Ability_Variables::ResetAbilityPause() {
    ability_pause = ABILITY_PAUSE_M;
    SaveAbilityPause();
}

void Ability_Variables::ResetCount() {
    count = 0;
    SaveAbilityPause();
}

void Ability_Variables::IncrementCount() {
    ++count;
    // Count changes every second, only persist it occasionally to prevent
    // too frequent eeprom writes.
    if (count % ABILUTY_SAVE_PAUSE == 0) {
        SaveCount();
    }
}

unsigned int Ability_Variables::GetCount() const {
    return count;
}

void Ability_Variables::SaveAbilityPause() {
    eeprom->Write(ability_pause, offsetof(EepromMap, vars2) + offsetof(Ability_Variables, ability_pause));
}
void Ability_Variables::SaveCount() {
    eeprom->Write(count, offsetof(EepromMap, vars2) + offsetof(Ability_Variables, count));
}
void Ability_Variables::SaveAbility() {
    eeprom->Write(ability, offsetof(EepromMap, vars2) + offsetof(Ability_Variables, ability));
}