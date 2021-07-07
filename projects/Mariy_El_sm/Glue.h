#include "qhsm.h"
#ifndef GLUE_H_
#define GLUE_H_

enum PlayerSignals {
    TICK_SEC_SIG = Q_USER_SIG,

    TIME_TICK_1M_SIG,
    TIME_TICK_1S_SIG,
    PILL_HEAL_SIG,
    PILL_RESET_SIG,
    PILL_GOD_SIG,
    LONG_PRESS_THIRD_SIG,
    MIDDLE_BUTTON_PRESSED_SIG,
    LONG_PRESS_SIG,
    RAD_RECEIVED_SIG,
    MONSTER_SIGNAl_SIG,
    LAST_USER_SIG [[maybe_unused]]
};

#endif /* GLUE_H_ */
