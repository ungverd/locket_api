//
// Created by juice on 12.10.2020.
//

#include <gtest/gtest.h>
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
    RxTable<IdAndTypeState, 10> table;
    std::vector<LedRGBChunk> sequence;
    CreateSequence(table.Raw(), sequence);
    EXPECT_EQ(sequence.size(), 1);
}

TEST(SequenceCreation, OnePacket) {
    RxTable<IdAndTypeState, 10> table;
    IdAndTypeState p = {1, 0};
    table.AddPacket(p);
    std::vector<LedRGBChunk> sequence;
    CreateSequence(table.Raw(), sequence);
    EXPECT_EQ(sequence.size(), 9);
    EXPECT_EQ(sequence[0].color, kYellow);
}

TEST(SequenceCreation, OneGroup) {
    RxTable<IdAndTypeState, 10> table;
    for (uint32_t i=0; i<5; i++) {
        table.AddPacket({i, 1});
    }
    std::vector<LedRGBChunk> sequence;
    CreateSequence(table.Raw(), sequence);
    EXPECT_EQ(sequence.size(), 41);
    EXPECT_EQ(sequence[0].color, kWhite);
}

TEST(SequenceCreation, SeveralGroups) {
    RxTable<IdAndTypeState, 10> table;
    for (uint32_t i=0; i<2; i++) {
        table.AddPacket({i, 2});
    }
    for (uint32_t i=2; i<5; i++) {
        table.AddPacket({i, 3});
    }
    for (uint32_t i=5; i<10; i++) {
        table.AddPacket({i, 4});
    }
    std::vector<LedRGBChunk> sequence;
    CreateSequence(table.Raw(), sequence);
    EXPECT_EQ(sequence.size(), 91);
    EXPECT_EQ(table.Raw().size(), 10);
    EXPECT_EQ(sequence[88].color, kBlack);
    EXPECT_EQ(sequence[87].color, kBlack);
    EXPECT_EQ(sequence[86].color, kBlue);
    EXPECT_EQ(sequence[85].color, kBlack);
    EXPECT_EQ(sequence[84].color, kRed);
    EXPECT_EQ(sequence[83].color, kBlack);
    EXPECT_EQ(sequence[82].color, kBlue);
    EXPECT_EQ(sequence[81].color, kBlack);
    EXPECT_EQ(sequence[80].color, kRed);
    EXPECT_EQ(sequence[79].color, kBlack);
}

TEST(Silent, SilentNotNear) {
    RxTable<IdAndTypeState, 10> table;
    for (uint32_t i=0; i<2; i++) {
        table.AddPacket({i, 2});
    }
    for (uint32_t i=2; i<5; i++) {
        table.AddPacket({i, 3});
    }
    for (uint32_t i=5; i<10; i++) {
        table.AddPacket({i, 4});
    }
    EXPECT_FALSE(IfSilentNear(table.Raw()));
}

TEST(Silent, SilentNear) {
    RxTable<IdAndTypeState, 10> table;
    for (uint32_t i=0; i<2; i++) {
        table.AddPacket({i, 2});
    }
    for (uint32_t i=2; i<5; i++) {
        table.AddPacket({i, 5});
    }
    for (uint32_t i=5; i<10; i++) {
        table.AddPacket({i, 4});
    }
    EXPECT_TRUE(IfSilentNear(table.Raw()));
}