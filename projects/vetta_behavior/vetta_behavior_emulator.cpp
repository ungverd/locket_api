#include "vetta_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<VettaBehavior>().Run();
}