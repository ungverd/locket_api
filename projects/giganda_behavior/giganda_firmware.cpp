#include "giganda_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<GigandaBehavior>().Run();
}