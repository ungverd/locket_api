#include "my_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<MyBehavior>().Run();
}