#ifndef LOCKET_API_RAD_BEHAVIOR_H
#define LOCKET_API_RAD_BEHAVIOR_H

#include "pill_manager.h"
#include "rx_table.h"
#include "behavior.h"



class RadBehavior: public Behavior<IdOnlyState, IdOnlyState> {
public:
    using Behavior::Behavior;

    void OnStarted() override;
    void EverySecond() override;
    void OnButtonPressed(uint16_t button_index, bool long_press) override;
    void OnPillConnected(PillManager<IdOnlyState>* manager) override;
    void OnPillDisconnected() override;

    //methods for logic
    void StartTransmitForPath();
    void SetColor(Color color);
    void MonsterVibro();
    void MakePillUsed();
    void RadiationVibro();
    void Flash(Color color_new, Color color_old);

private:
    PillManager<IdOnlyState>* pill_manager = nullptr;
    const unsigned int path_id = 1;
    const unsigned int used_pill_id = 10;
};

#endif //LOCKET_API_SM_BEHAVIOR_H
