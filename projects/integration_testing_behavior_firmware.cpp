#include "integration_testing_behavior.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<IntegrationTestingBehavior>().Run();
}