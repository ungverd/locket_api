//
// Created by juice on 02.08.2020.
//

#ifndef LOCKET_API_TRY_GIGANDA_H
#define LOCKET_API_TRY_GIGANDA_H

#include "behavior.h"
#include "pill_manager.h"

enum class LocketEnum{NOTHING, ACCELERATOR, REGENERATOR, REFRIGERATOR};
enum class PillEnum{EMPTY, ACCELERATE, REGENERATE, REFRIGERATE, LIGHT, BOMB};

class GigandaBehavior: public Behavior<IdOnlyState, EmptyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void OnPillConnected(PillManager<IdOnlyState>* manager) override;
    void OnPillDisconnected() override;
    void OnButtonPressed(uint16_t button_index) override;
    void OnDipSwitchChanged(uint16_t dip_value_mask) override;


private:
    PillManager<IdOnlyState>* pill_manager = nullptr;
    LocketEnum LocketType = LocketEnum::NOTHING;

};

#endif //LOCKET_API_TRY_GIGANDA_H
