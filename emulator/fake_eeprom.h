#ifndef LOCKET_API_FAKE_EEPROM_H
#define LOCKET_API_FAKE_EEPROM_H

#include "api/eeprom.h"

class FakeEeprom: public Eeprom {
public:
    uint32_t ReadUint32(uint32_t address) override;
    uint8_t WriteUint32(uint32_t address, uint32_t value) override;

private:
    uint32_t memory[1000];
};


#endif //LOCKET_API_FAKE_EEPROM_H
