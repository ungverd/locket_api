#include "vibro_wrapper.h"

void VibroWrapper::StartOrRestart(const BaseChunk *sequence) {
    implementation->StartOrRestart(sequence);
}
