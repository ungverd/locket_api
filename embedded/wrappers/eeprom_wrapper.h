#ifndef LOCKET_API_EEPROM_WRAPPER_H
#define LOCKET_API_EEPROM_WRAPPER_H

#include "api/eeprom.h"

class EepromWrapper: public Eeprom {
public:
    uint32_t ReadUint32(uint32_t address) override;
    uint8_t WriteUint32(uint32_t address, uint32_t value) override;
};


#endif //LOCKET_API_EEPROM_WRAPPER_H
