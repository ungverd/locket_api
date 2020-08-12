#include <gtest/gtest.h>
#include "rx_table.h"

struct DummyRadioPacket {
    bool operator==(const DummyRadioPacket& other) const {
        return id == other.id && something_else == other.something_else && stuff == other.stuff;
    }

    uint8_t id;
    char something_else;
    uint32_t stuff;
};

class RxTableTest: public testing::Test {
public:
    RxTable<DummyRadioPacket, 10> table;
};

TEST_F(RxTableTest, CanAddPacket) {
    DummyRadioPacket p = {10, 'x', 123};
    EXPECT_EQ(table.AddPacket(p), true);
    EXPECT_EQ(table.Raw().size(), 1);
    EXPECT_EQ(table.Raw()[0], p);
}

TEST_F(RxTableTest, LatestPacketOverridesPrevious) {
    DummyRadioPacket p_first = {10, 'x', 123};
    DummyRadioPacket p_second = {10, 'y', 321};
    EXPECT_EQ(table.AddPacket(p_first), true);
    EXPECT_EQ(table.AddPacket(p_second), true);
    EXPECT_EQ(table.Raw().size(), 1);
    EXPECT_EQ(table.Raw()[0], p_second);
}

TEST_F(RxTableTest, CanGetPacketByIdIfPresent) {
    DummyRadioPacket p1 = {11, 'x', 123};
    DummyRadioPacket p2 = {12, 'y', 321};
    DummyRadioPacket p3 = {13, 'z', 555};
    EXPECT_EQ(table.AddPacket(p1), true);
    EXPECT_EQ(table.AddPacket(p2), true);
    EXPECT_EQ(table.AddPacket(p3), true);

    {
        const DummyRadioPacket* p = table.GetPacketById(12);
        EXPECT_NE(p, nullptr);
        EXPECT_EQ(*p, p2);
    }

    {
        const DummyRadioPacket* p = table.GetPacketById(14);
        EXPECT_EQ(p, nullptr);
    }
}

TEST_F(RxTableTest, CanCheckForPacketPresence) {
    DummyRadioPacket p1 = {11, 'x', 123};
    DummyRadioPacket p2 = {12, 'y', 321};
    DummyRadioPacket p3 = {13, 'z', 555};
    EXPECT_EQ(table.AddPacket(p1), true);
    EXPECT_EQ(table.AddPacket(p2), true);
    EXPECT_EQ(table.AddPacket(p3), true);
    EXPECT_TRUE(table.HasPacketWithId(12));
    EXPECT_FALSE(table.HasPacketWithId(14));
}

TEST_F(RxTableTest, AddPacketReturnsFalseOnOverflow) {
    for (uint8_t i = 0; i < 10; ++i) {
        DummyRadioPacket p = {i, 'x', 123};
        EXPECT_TRUE(table.AddPacket(p));
        EXPECT_EQ(table.Raw().size(), i + 1);
    }

    DummyRadioPacket p = {11, 'x', 123};
    EXPECT_FALSE(table.AddPacket(p));
    EXPECT_EQ(table.Raw().size(), 10);
}

TEST_F(RxTableTest, CanFitAnyNumbersOfPacketsWithSameId) {
    for (uint32_t i = 0; i < 100; ++i) {
        DummyRadioPacket p = {10, 'x', i};
        EXPECT_TRUE(table.AddPacket(p));
        EXPECT_EQ(table.Raw().size(), 1);
    }
}

TEST_F(RxTableTest, CanClear) {
    DummyRadioPacket p1 = {11, 'x', 123};
    DummyRadioPacket p2 = {12, 'y', 321};
    DummyRadioPacket p3 = {13, 'z', 555};
    EXPECT_EQ(table.AddPacket(p1), true);
    EXPECT_EQ(table.AddPacket(p2), true);
    EXPECT_EQ(table.AddPacket(p3), true);

    table.Clear();
    EXPECT_EQ(table.Raw().size(), 0);
    EXPECT_FALSE(table.HasPacketWithId(12));
}