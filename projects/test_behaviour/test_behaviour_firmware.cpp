#include "projects/test_behaviour/test_behaviour.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<TestBehavior>().Run();
}