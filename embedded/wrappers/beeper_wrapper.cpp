#include "beeper_wrapper.h"

void BeeperWrapper::StartOrRestart(const BeepChunk *sequence) {
    implementation->StartOrRestart(sequence);
}
