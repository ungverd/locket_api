#include "beeper_wrapper.h"

void BeeperWrapper::StartOrRestart(const BeepChunk *sequence) {
    implementation->StartOrRestart(sequence);
}

void BeeperWrapper::StartOrContinue(const BeepChunk* sequence) {
    implementation->StartOrContinue(sequence);
}

void BeeperWrapper::Stop() {
    implementation->Stop();
}
