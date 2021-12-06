#ifndef LOCKET_API_LEO_BEHAVIOR_H
#define LOCKET_API_LEO_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"

struct IdAndLampParameters {
    uint8_t id;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint16_t delay;
};


class LeoBehavior: public Behavior<IdOnlyState, IdAndLampParameters> {
public:
    enum class Mode {
        lamp = 0,
        control = 1,
    };

    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;
    void OnButtonPressed(uint16_t button_index, bool long_press) override;
    void OnRadioPacketReceived(const IdAndLampParameters& packet, int8_t rssi) override;
    void OnUartCommand(UartCommand& command) override;

private:
    Mode mode = Mode::lamp;
    RxTable<IdAndLampParameters> rx_table;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t probability = 0;
    uint16_t delay = 0;
};

#endif //LOCKET_API_LEO_BEHAVIOR_H
