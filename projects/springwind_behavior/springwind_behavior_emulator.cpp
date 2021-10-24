#include "springwind_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<WindsBehavior>().Run();
}