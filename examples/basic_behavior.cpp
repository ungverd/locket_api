#include "behavior.h"
#include "pill_manager.h"
#include <iostream>

class BasicBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    void OnStarted() override {
        std::cout << "Started execution!" << std::endl;
    }

private:
    PillManager<IdOnlyState>* pill_manager;
};