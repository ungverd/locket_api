#include "vibro_wrapper.h"

void VibroWrapper::StartOrRestart(const VibroChunk* sequence) {
    implementation->StartOrRestart(sequence);
}
