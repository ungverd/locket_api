//
// Created by juice on 21.08.2020.
//
#include "feeling_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<FeelingBehavior>().Run();
}

