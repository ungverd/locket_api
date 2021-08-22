//
// Created by juice on 18.07.2021.
//

#define FALLOUT_DEVICE

#include "magic_path_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<MagicPathBehavior>().Run();
}