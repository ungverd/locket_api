//
// Created by juice on 12.10.2020.
//

#include <../cmake-build-debug/googletest-src/googletest/include/gtest/gtest.h>
#include "rx_table.h"
#include "kirlits.h"

struct KirlitsDummyRadioPacket {
    bool operator==(const KirlitsDummyRadioPacket& other) const {
        return id == other.id && group == other.group;
    }

    uint8_t id;
    uint32_t group;
};

TEST(SequenceCreation, EmptySequense) {
    RxTable<KirlitsDummyRadioPacket, 10> table;
}

