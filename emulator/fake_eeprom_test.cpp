#include <gtest/gtest.h>
#include "fake_eeprom.h"

namespace {

struct EepromData {
    uint32_t a, b, c;
};

}

TEST(FakeEeprom, CanWriteAndReadBack) {
    FakeEeprom eeprom;
    EXPECT_EQ(eeprom.WriteUint32(offsetof(EepromData, b), 156), 0);
    EXPECT_EQ(eeprom.ReadUint32(offsetof(EepromData, b)), 156);
}
