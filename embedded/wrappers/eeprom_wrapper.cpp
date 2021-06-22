#include "eeprom_wrapper.h"

#include "embedded/kl_lib/kl_lib.h"

uint32_t EepromWrapper::ReadUint32(uint32_t address) {
    return EE::Read32(address);
}

uint8_t EepromWrapper::WriteUint32(uint32_t address, uint32_t value) {
    return EE::Write32(address, value);
}
