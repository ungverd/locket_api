#include "eeprom_wrapper.h"

#include "embedded/kl_lib/kl_lib.h"

namespace {
    // First 512 bytes are reserved for low-level data.
    uint32_t kHighLevelDataOffset = 512;
}

void EepromWrapper::ReadImpl(void* destination, uint32_t size, uint32_t address) {
    return EE::ReadBuf(destination, size, kHighLevelDataOffset + address);
}

uint8_t EepromWrapper::WriteImpl(void* source, uint32_t size, uint32_t address) {
    return EE::WriteBuf(source, size, kHighLevelDataOffset + address);
}
