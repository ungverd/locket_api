#ifndef LOCKET_API_EEPROM_H
#define LOCKET_API_EEPROM_H

#include <cstdint>

// Allows to write to and read from EEPROM (memories there are persisted when device e.g. reboots).
// For now, only unsigned 32-bit values are supported.
//
// Use the following construction to calculate addresses if you need to store multiple values in EEPROM:
// struct EepromData {
//     uint32_t id;
//     uint32_t a;
//     uint32_t b;
//     uint32_t c;
// };
// ...
// uin32_t a = eeprom->ReadUint32(offsetof(EepromData, a));
// uint32_t b = 56;
// eeprom->WriteUint32(offsetof(EepromData, b), b);
class Eeprom {
public:
    virtual uint32_t ReadUint32(uint32_t address) = 0;

    // Returns error code (0 on success). Caller must check the return value and handle non-zero error codes.
    [[nodiscard]] virtual uint8_t WriteUint32(uint32_t address, uint32_t value) = 0;
};

#endif //LOCKET_API_EEPROM_H
