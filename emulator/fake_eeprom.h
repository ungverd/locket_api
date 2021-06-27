#ifndef LOCKET_API_FAKE_EEPROM_H
#define LOCKET_API_FAKE_EEPROM_H

#include "api/eeprom.h"

class FakeEeprom: public Eeprom {
private:
    void ReadImpl(void* destination, uint32_t size, uint32_t address) override;
    uint8_t WriteImpl(void* source, uint32_t size, uint32_t address) override;
    uint8_t memory[1000];
};


#endif //LOCKET_API_FAKE_EEPROM_H
