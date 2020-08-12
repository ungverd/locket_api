#ifndef LOCKET_API_RX_TABLE_H
#define LOCKET_API_RX_TABLE_H

#include <algorithm>
#include <vector>

// Table to store received radio packets.
// Typical usage pattern is the following:
// * Whenever radio packet is received, call AddPacket.
// * Once in a while (e.g. every second) check the contents using  HasPacketWithId/GetPacketById/Raw methods and
//   call Clear().
//
// TRadioPacket must have 'uint8_t id' member so we can compare packets by id.
// TableSize is a maximal number of packets with different ids RxTable can fit.
//   If too large - table will take too much RAM.
//   If too small - if we receive a lot of packets with different ids between Clear() calls - some of them will be lost.
template <typename TRadioPacket, int TableSize = 16>
class RxTable {
public:
    RxTable();

    // Adds packet to the table. If packet with the same id already present - overwrites it.
    // Returns true iff packet fits into table, false on table overflow.
    bool AddPacket(const TRadioPacket& packet);

    // Clears the table. Must be called periodically as table size is limited.
    void Clear();

    // Return raw contents of table for the inspection.
    const std::vector<TRadioPacket>& Raw() const;

    // Returns the pointer to the packet with the specified id if present.
    // If no packet with such id is present - returns nullptr.
    const TRadioPacket* GetPacketById(uint8_t id) const;

    // Returns true iff packet with provided id is present in the table.
    bool HasPacketWithId(uint8_t id) const;

private:
    // We create a vector of size TableSize here and then reset it in constructor. That prevents re-allocations due to
    // vector growth. Technically, table.reserve in constructor is enough for that, but we would like to give a
    // compiler additional hint.
    std::vector<TRadioPacket> table{TableSize};
};

template<typename TRadioPacket, int TableSize>
bool RxTable<TRadioPacket, TableSize>::AddPacket(const TRadioPacket& packet) {
    auto it = std::find_if(std::begin(table), std::end(table),
                           [&](const TRadioPacket& p) { return p.id == packet.id; });
    if (it == std::end(table)) {
        if (table.size() < TableSize) {
            table.push_back(packet);
            return true;
        } else {
            return false;
        }
    } else {
        *it = packet;
        return true;
    }
}

template<typename TRadioPacket, int TableSize>
RxTable<TRadioPacket, TableSize>::RxTable() {
    table.reserve(TableSize);
    table.clear();
}

template<typename TRadioPacket, int TableSize>
void RxTable<TRadioPacket, TableSize>::Clear() {
    table.clear();
}

template<typename TRadioPacket, int TableSize>
const std::vector<TRadioPacket>& RxTable<TRadioPacket, TableSize>::Raw() const {
    return table;
}

template<typename TRadioPacket, int TableSize>
const TRadioPacket* RxTable<TRadioPacket, TableSize>::GetPacketById(uint8_t id) const {
    auto it = std::find_if(std::begin(table), std::end(table),
                           [&](const TRadioPacket& p) { return p.id == id; });
    if (it != std::end(table)) return &*it;
    return nullptr;
}

template<typename TRadioPacket, int TableSize>
bool RxTable<TRadioPacket, TableSize>::HasPacketWithId(uint8_t id) const {
    return GetPacketById(id) != nullptr;
}

#endif //LOCKET_API_RX_TABLE_H
