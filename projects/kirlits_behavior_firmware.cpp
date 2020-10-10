//
// Created by juice on 28.09.2020.
//
#include "kirlits.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<KirlitsBehavior>().Run();
}

