#include "fake_eeprom.h"
#include <cstring>

void FakeEeprom::ReadImpl(void* destination, uint32_t size, uint32_t address) {
    std::memcpy(destination, memory + address, size);
}

uint8_t FakeEeprom::WriteImpl(void* source, uint32_t size, uint32_t address) {
    std::memcpy(memory + address, source, size);
    return 0;
}
