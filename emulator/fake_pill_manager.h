#ifndef LOCKET_API_EMULATOR_FAKE_PILL_MANAGER_H
#define LOCKET_API_EMULATOR_FAKE_PILL_MANAGER_H

#include "pill_manager.h"

// For now, supports single pill with IdOnlyState.
// TODO(aeremin): Improve by adding support for multiple pills.
class FakePillManager: public PillManager<IdOnlyState> {
public:
    IdOnlyState ReadPill() override;
    void WritePill(const IdOnlyState &state) override;

private:
    IdOnlyState state = {/* id = */ 0};
};

#endif //LOCKET_API_EMULATOR_FAKE_PILL_MANAGER_H
