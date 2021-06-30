#include "mariel_sm.h"
#include "emulator.h"

int main() {
    emulator::BehaviorRunner<RadBehavior>().Run();
}