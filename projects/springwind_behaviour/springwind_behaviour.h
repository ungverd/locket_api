#ifndef LOCKET_API_BASIC_BEHAVIOR_H
#define LOCKET_API_BASIC_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"

uint32_t FiveMinThreshold = 300;
uint32_t FifteenThreshold = 900;
uint32_t HalfAnHourThreshold = 1800;

class WindsBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    enum class Mode {
        kPlayer = 0,
        kLadyLu = 1,
        kMasterSos = 2
    };

    enum class State {
        kIdle = 0,
        kFive = 1,
        kFifteen = 2,
        kHalfAnHour = 3,
        kActivated = 4
    };
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;
    void OnButtonPressed(uint16_t button_index, bool long_press) override;
    void OnRadioPacketReceived(const IdOnlyState& packet, int8_t rssi) override;
    void OnUartCommand(UartCommand& command) override;

private:
    uint32_t seconds_counter = 0;
    uint32_t current_threshold = 0;
    Mode mode = Mode::kPlayer;
    State state = State::kIdle;
    RxTable<IdOnlyState> rx_table;
    RadioPowerLevel RangeLevel;
};

#endif //LOCKET_API_BASIC_BEHAVIOR_H
