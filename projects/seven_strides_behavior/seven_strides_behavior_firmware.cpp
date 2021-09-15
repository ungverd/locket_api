//
// Created by juice on 15.09.2021.
//

#include "seven_strides_behaviour.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<SevenStridesBehavior>().Run();
}