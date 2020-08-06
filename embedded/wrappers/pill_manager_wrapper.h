#ifndef LOCKET_API_PILL_MANAGER_WRAPPER_H
#define LOCKET_API_PILL_MANAGER_WRAPPER_H

#include "api/pill_manager.h"
#include "embedded/Pill/pill_mgr.h"

template <typename PillState>
class PillManagerWrapper: public PillManager<PillState> {
public:
    explicit PillManagerWrapper(PillMgr_t* implementation): impl(implementation) {}

    PillState ReadPill() override {
        PillState result;
        impl->Read(0, &result, sizeof(PillState));
        return result;
    }

    void WritePill(const PillState& state) override {
        impl->Write(0, &state, sizeof(PillState));
    }

private:
    PillMgr_t* impl;
};


#endif //LOCKET_API_PILL_MANAGER_WRAPPER_H
