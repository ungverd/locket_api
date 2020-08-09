//
// Created by juice on 02.08.2020.
//

#ifndef LOCKET_API_GIGANDA_BEHAVIOR_H
#define LOCKET_API_GIGANDA_BEHAVIOR_H

#define ACCELERATE_S 30
#define REGENERATE_S  10
#define REFRIGERATE_S  15
#define BOMB_S  60

#include "behavior.h"
#include "pill_manager.h"
#include "sequences.h"

enum class LocketEnum{NOTHING, ACCELERATOR, REGENERATOR, REFRIGERATOR, MASTER};
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
    uint8_t master_pill_id = 0;

    PillEnum IdToEnum(uint8_t id);
    uint8_t GetNextState(uint8_t pill_id);
};

#endif //LOCKET_API_GIGANDA_BEHAVIOR_H
