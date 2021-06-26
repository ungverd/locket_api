//
// Created by juice on 21.08.2020.
//
#include "feeling_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<FeelingBehavior>().Run();
}
