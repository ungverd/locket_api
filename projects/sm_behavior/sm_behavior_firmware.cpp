#include "sm_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<StateMachineBehavior>().Run();
}