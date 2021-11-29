#include "player_device.h"
#include "lustra.h"
#include "embedded/behavior_runner.h"
#include "utility.h"

int main() {
    uint8_t first_dip = GetSwitchState(embedded::ReadDipSwitch(), 1);
    if (first_dip) {
        embedded::BehaviorRunner<LustraBehavior>().Run();
    } else {
        embedded::BehaviorRunner<RadBehavior>().Run();
    };
}