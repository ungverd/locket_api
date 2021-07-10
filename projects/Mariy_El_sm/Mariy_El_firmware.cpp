#include "player_device.h"
#include "monster.h"
#include "lustra.h"
#include "embedded/behavior_runner.h"
#include "utility.h"

int main() {
    uint8_t first_dip = GetSwitchState(embedded::ReadDipSwitch(), 1);
    uint8_t second_dip = GetSwitchState(embedded::ReadDipSwitch(), 2);
    if (not first_dip and second_dip) {
        embedded::BehaviorRunner<MonsterBehavior>().Run();
    } else if (first_dip and not second_dip) {
        embedded::BehaviorRunner<LustraBehavior>().Run();
    } else {
        embedded::BehaviorRunner<RadBehavior>().Run();
    };
}