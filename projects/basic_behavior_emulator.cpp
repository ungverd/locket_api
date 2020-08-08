#include "basic_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<BasicBehavior>().Run();
}