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
// uin32_t a = eeprom->Read<uin32_t>(offsetof(EepromData, a));
// uint32_t b = 56;
// eeprom->Write(offsetof(EepromData, b), b);
class Eeprom {
public:
    // Reads value of type T.
    // Example usage:
    //   uin32_t a = eeprom->Read<uin32_t>(offsetof(EepromData, a));
    template<typename T> T Read(uint32_t address);

    // Tries to write value of type T.
    // Example usage:
    //  uint32_t b = 56;
    //  eeprom->Write(offsetof(EepromData, b), b);
    // Returns error code (0 on success). Caller must check the return value and handle non-zero error codes.
    template<typename T> uint8_t Write(uint32_t address, T value);

private:
    virtual void ReadImpl(void* destination, uint32_t size, uint32_t address) = 0;
    virtual uint8_t WriteImpl(void* source, uint32_t size, uint32_t address) = 0;
};

template<typename T>
T Eeprom::Read(uint32_t address) {
    T result;
    ReadImpl(&result, sizeof(T), address);
    return result;
}

template<typename T>
uint8_t Eeprom::Write(uint32_t address, T value) {
    return WriteImpl(&value, sizeof(T), address);
}

#endif //LOCKET_API_EEPROM_H
