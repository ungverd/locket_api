#include "player_device.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<RadBehavior>().Run();
}