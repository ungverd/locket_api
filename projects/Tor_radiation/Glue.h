#include "qhsm.h"
#ifndef GLUE_H_
#define GLUE_H_

const uint32_t rad_id = 3;

enum PlayerSignals {
    TICK_SEC_SIG = Q_USER_SIG,

    TIME_TICK_1M_SIG,
    TIME_TICK_1S_SIG,
    BTN_RESET_SIG,
    RAD_RECEIVED_SIG,
    LAST_USER_SIG [[maybe_unused]]
};

#endif /* GLUE_H_ */
