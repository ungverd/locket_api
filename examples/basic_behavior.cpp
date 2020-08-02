#include "behavior.h"
#include "pill_manager.h"
#include <iostream>

class BasicBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;
    void OnStarted() override {
        std::cout << "Started execution!" << std::endl;
        led->StartOrRestart(RgbLedSequence());
    }

    void OnPillConnected(PillManager<IdOnlyState>* manager) override {
        pill_manager = manager;
    }

    void OnPillDisconnected() override {
        pill_manager = nullptr;
    }

private:
    PillManager<IdOnlyState>* pill_manager;
};

// Just to make sure it compiles
BasicBehavior b(nullptr);
