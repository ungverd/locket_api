//
// Created by juice on 15.09.2021.
//

#include "springwind_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<WindsBehavior>().Run();
}