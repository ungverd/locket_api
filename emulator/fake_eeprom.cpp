#include "fake_eeprom.h"

uint32_t FakeEeprom::ReadUint32(uint32_t address) {
    if (address < 0 || address >= 1000 * sizeof(uint32_t) || address % sizeof(uint32_t) != 0) return 0;
    return memory[address / sizeof(uint32_t)];
}

uint8_t FakeEeprom::WriteUint32(uint32_t address, uint32_t value) {
    if (address < 0 || address >= 1000 * sizeof(uint32_t) || address % sizeof(uint32_t) != 0) return -1;
    memory[address / sizeof(uint32_t)] = value;
    return 0;
}
