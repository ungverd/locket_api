#include "giganda_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<GigandaBehavior>().Run();
}

