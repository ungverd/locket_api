#ifndef LOCKET_API_MAGIC_PATH_BEHAVIOR_H
#define LOCKET_API_MAGIC_PATH_BEHAVIOR_H

#include "rx_table.h"
#include "behavior.h"

class MagicPathBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) override;

private:
    RxTable<IdOnlyState> rx_table;
    unsigned int count = 0;
};

#endif //LOCKET_API_MAGIC_PATH_BEHAVIOR_H
