#ifndef LOCKET_API_EEPROM_WRAPPER_H
#define LOCKET_API_EEPROM_WRAPPER_H

#include "api/eeprom.h"

class EepromWrapper: public Eeprom {
public:
    void ReadImpl(void* destination, uint32_t size, uint32_t address) override;
    uint8_t WriteImpl(void* source, uint32_t size, uint32_t address) override;
};


#endif //LOCKET_API_EEPROM_WRAPPER_H
