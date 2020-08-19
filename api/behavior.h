#ifndef LOCKET_API_BEHAVIOR_H
#define LOCKET_API_BEHAVIOR_H

#include "common_states.h"
#include "led.h"
#include "pill_manager.h"
#include "logger.h"
#include "vibro.h"
#include "beeper.h"
#include "radio.h"

// Business logic developer needs to
// 1) Decide what's the structure of data
//    - Stored on the pill.
//    - Transmitted over radio.
//    See common_states.h for the examples struct's.
//    If something more complex is needed - define your own struct (e.g. ThatGamePillState).
//    Note: only fixed-size data is allowed in those structs:
//    - int32_t id - good, 32 bits of data.
//    - int8_t stuff[3] - good, 24 bits of data.
//    - std::vector<int8_t> v - bad.
//    - std::string s - bad (but you can potentially use 'char s[10]' if string size is limited to 10).
//    - int8_t* something - bad.
// 2) Create a class implementing Behavior interface, e.g.
//    class ThatGameBehavior: public Behavior<ThatGamePillState, EmptyState> {
//    public:
//        using Behavior::Behavior; // This is magic needed to inherit a constructor, just do it.
//    };
//    (this is for the case when custom struct ThatGamePillState describes pills content and there is no radio).
// 3) Override On[Something] methods if you want to react on corresponding events, e.g.
//    class ThatGameBehavior: public Behavior<ThatGamePillState, EmptyState> {
//    public:
//        void OnStarted() override {
//           ...
//        }
//    };
//    All On[Something] methods have a default behavior of "do nothing", so there is no need to override all methods,
//    only ones that you are interested in.
//
template <typename PillState = EmptyState, typename RadioPacket = EmptyState>
class Behavior {
public:
    //
    // General events
    //

    // Will be called after startup initialization is complete.
    // Note: it is guaranteed that OnDipSwitchChanged will be called once before calling OnStarted. So you can
    // init thing like device kind/id/etc. in OnDipSwitchChanged and then use them in OnStarted (e.g. to show different
    // startup indications).
    virtual void OnStarted() {};

    // Will be called once per second.
    virtual void EverySecond() {};

    //
    // Pill-related events
    //
    // If you only need to read pill content - just do
    //   manager->ReadPill() in OnPillConnected to get contents.
    // If you need to read/write "later" (e.g. on button press) - define a
    //   PillManager<IdOnlyState>* pill_manager = nullptr;
    // member of your Behavior class, do a
    //   pill_manager = manager;
    // in a OnPillConnected,
    //   pill_manager = nullptr;
    // in OnPillDisconnected.
    // It will give you a nice way to check if pill is connected by checking if pill_manager != nullptr. So in e.g.
    // OnButtonPressed you can do
    //   if (pill_manager) {
    //     pill_manager->WritePill(...)
    //   } else {
    //     // Pill not connected, don't do anything/indicate an error.
    //   }
    virtual void OnPillConnected(PillManager<PillState>* manager) {}
    virtual void OnPillDisconnected() {}

    //
    // Button-handling events.
    //
    // For now, only single-button presses are supported (no combinations, no long presses, etc).
    virtual void OnButtonPressed(uint16_t button_index) {}

    // DIP switch event. Will be called if any of toggles on DIP switch are flipped.
    // To get the ON/OFF state of the specific switch, use GetSwitchState from the utility.h.
    virtual void OnDipSwitchChanged(uint16_t dip_value_mask) {}

    // Will be called each time new radio packet is received.
    // Most typical way to handle that is to
    // 1) Add `RxTable<RadioPacket> rx_table` member to your behavior class.
    // 2) Call `rx_table.AddPacket(packet)` in OnRadioPacketReceived.
    // 3) Investigate recent packets in EverySecond using rx_table methods and then call rx_table.Clear().
    //
    // In this approach, RxTable is used to deduplicate packets with same id. Also, we don't process incoming packets
    // immediately and independently, but instead process all packets received during recent second. This simplifies
    // implementation of more complex behaviors
    virtual void OnRadioPacketReceived(const RadioPacket& packet) {}

    // Internal: This is needed for BehaviorRunner to work.
    using PillStateParameter = PillState;
    using RadioPacketParameter = RadioPacket;

protected:
    // Use this to print to UART/console.
    Logger* const logger;

    // Use this to control LED.
    RgbLed* const led;

    // Use this to control LED.
    Beeper* const beeper;

    // Use this to control LED.
    Vibro* const vibro;

    // Use this to send radio packets.
    Radio<RadioPacket>* const radio;

public:
    // Concrete Behavior implementations should either inherit it by having
    //     using Behavior::Behavior;
    // in their public section, or (advanced mode) define their own constructor and call this one there.
    Behavior(Logger* logger, RgbLed* led, Beeper* beeper, Vibro* vibro, Radio<RadioPacket>* radio):
            logger(logger), led(led), beeper(beeper), vibro(vibro), radio(radio) {}
};

#endif //LOCKET_API_BEHAVIOR_H
