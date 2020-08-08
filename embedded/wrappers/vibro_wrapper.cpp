#include "vibro_wrapper.h"

void VibroWrapper::StartOrRestart(const VibroChunk* sequence) {
    implementation->StartOrRestart(sequence);
}

void VibroWrapper::StartOrContinue(const VibroChunk* sequence) {
    implementation->StartOrContinue(sequence);
}

void VibroWrapper::Stop() {
    implementation->Stop();
}
