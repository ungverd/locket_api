#include "eeprom_wrapper.h"

#include "embedded/kl_lib/kl_lib.h"

namespace {
    // First 512 bytes are reserved for low-level data.
    uint32_t kHighLevelDataOffset = 512;
}

uint32_t EepromWrapper::ReadUint32(uint32_t address) {
    return EE::Read32(kHighLevelDataOffset + address);
}

uint8_t EepromWrapper::WriteUint32(uint32_t address, uint32_t value) {
    return EE::Write32(kHighLevelDataOffset + address, value);
}
