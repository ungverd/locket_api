#ifndef health_h
#define health_h

#include "player_device.h"
#include "api/eeprom.h"
#include "sm_data.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "qhsm.h"    /* include own framework tagunil version */


//Start of h code from diagram
const uint32_t HEALTH = 0;
const uint32_t DAMAGED = 1;
const uint32_t DEAD = 2;
const uint32_t MIN_TIMEOUT_S = 6000;
const uint32_t MAX_TIMEOUT_S = 170*60;


//End of h code from diagram



typedef struct {
/* protected: */
    QHsm super;
    RadBehavior* SMBeh;

/* public: */
    QStateHandler StartState;
    Health_Variables vars;
    Eeprom* eeprom;
    Logger* logger;
} Health;

/* protected: */
QState Health_initial(Health * const me, QEvt const * const e);
QState Health_global(Health * const me, QEvt const * const e);
QState Health_damaged(Health * const me, QEvt const * const e);
QState Health_health(Health * const me, QEvt const * const e);
QState Health_dead(Health * const me, QEvt const * const e);

#ifdef DESKTOP
QState Health_final(Health * const me, QEvt const * const e);
#endif /* def DESKTOP */


static Health health; /* the only instance of the Health class */

typedef struct healthQEvt {
    QEvt super;
} healthQEvt;


extern QHsm * const the_health; /* opaque pointer to the health HSM */

void Health_ctor(RadBehavior *SMBeh, Eeprom* eeprom, Logger* logger);
#ifdef __cplusplus
}
#endif
#endif /* health_h */