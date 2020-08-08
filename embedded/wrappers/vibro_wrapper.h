#ifndef LOCKET_API_VIBRO_WRAPPER_H
#define LOCKET_API_VIBRO_WRAPPER_H

#include "kl_lib/vibro.h"
#include "api/vibro.h"

class VibroWrapper: public Vibro {
public:
    explicit VibroWrapper(Vibro_t* implementation): implementation(implementation) {}
    void StartOrRestart(const VibroChunk* sequence) override;
    void StartOrContinue(const VibroChunk *sequence) override;
    void Stop() override;

private:
    Vibro_t* const implementation;
};
#endif //LOCKET_API_VIBRO_WRAPPER_H
