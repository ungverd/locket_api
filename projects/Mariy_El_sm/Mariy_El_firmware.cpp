#include "mariel_sm.h"
#include "embedded/behavior_runner.h"

int main() {
    embedded::BehaviorRunner<RadBehavior>().Run();
}