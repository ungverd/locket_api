#include "sm_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<StateMachineBehavior>().Run();
}