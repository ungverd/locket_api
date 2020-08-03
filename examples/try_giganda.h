//
// Created by juice on 02.08.2020.
//

#ifndef LOCKET_API_TRY_GIGANDA_H
#define LOCKET_API_TRY_GIGANDA_H

#define ACCELERATE_MS 30
#define REGENERATE_MS  10
#define REFRIGERATE_MS  15
#define BOMB_MS  60

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
    void EverySecond() override;


private:
    PillManager<IdOnlyState>* pill_manager = nullptr;
    LocketEnum LocketType = LocketEnum::NOTHING;
    uint32_t state_timer = 0;
    bool alive = true;

    PillEnum IdToEnum(uint8_t id);


};

#endif //LOCKET_API_TRY_GIGANDA_H
