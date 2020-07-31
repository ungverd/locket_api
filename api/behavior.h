#ifndef LOCKET_API_BEHAVIOR_H
#define LOCKET_API_BEHAVIOR_H

#include "common_states.h"

// Business logic developer needs to
// 1) Figure out what kind of data needs to be stored on the pill and which one needs to be transmitted over radio.
//    See common_states.h for the examples.
//    If something more complex is needed - define your own struct (e.g. ThatGamePillState).
// 2) Create a class implementing Behavior interface, e.g.
//    class ThatGameBehavior: public Behavior<ThatGamePillState, EmptyState> { ... };
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
    // Will be called after startup initialization is complete.
    virtual void OnStarted() {};
};

#endif //LOCKET_API_BEHAVIOR_H
