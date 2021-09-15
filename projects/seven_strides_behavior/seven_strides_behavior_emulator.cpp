//
// Created by juice on 15.09.2021.
//

#include "seven_strides_behaviour.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<SevenStridesBehavior>().Run();
}
