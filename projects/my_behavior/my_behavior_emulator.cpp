#include "my_behavior.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<MyBehavior>().Run();
}