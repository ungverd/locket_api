#include "leo_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<LeoBehavior>().Run();
}