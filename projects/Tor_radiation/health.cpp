#include "qhsm.h"
#include "health.h"
#include "Glue.h"
#include "color.h"

QHsm * const the_health = (QHsm *) &health; /* the opaque pointer */

void Health_ctor(
    RadBehavior *SMBeh, Eeprom* eeprom, Logger* logger)
{
    Health *me = &health;
    me->eeprom = eeprom;
    me->logger = logger;
    me->SMBeh = SMBeh;
    unsigned int State = eeprom->Read<unsigned int>(offsetof(EepromMap, health_state));
    me->vars = Health_Variables::Load(eeprom);
    switch (State) {
        case HEALTH: {
            me->StartState = (QStateHandler)&Health_health;
            break;
        }
        case DAMAGED: {
            me->StartState = (QStateHandler)&Health_damaged;
            break;
        }
        case DEAD: {
            me->StartState = (QStateHandler)&Health_dead;
            break;
        }
        default:
            me->StartState = (QStateHandler)&Health_health;
        }
    QHsm_ctor(&me->super, Q_STATE_CAST(&Health_initial));
}

QState Health_initial(Health * const me, [[maybe_unused]] QEvt const * const e) {
    /*${SMs::Health::SM::initial} */
    return Q_TRAN(me->StartState);
}
/*${SMs::Health::SM::global} ...............................................*/
QState Health_global(Health * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {

#ifdef DESKTOP
        /*${SMs::Health::SM::global::TERMINATE} */
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&Health_final);
            break;
        }
#endif /* def DESKTOP */
        case BTN_RESET_SIG: {
            status_ = Q_TRAN(&Health_health);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

QState Health_damaged(Health * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->logger->log("Entered state damaged");
            status_ = Q_HANDLED();
            // todo add randomizer
            unsigned int new_count = MIN_TIMEOUT_S;
            me->vars.SetCount(new_count);
            me->SMBeh->SetColor(kLightGreen);
            me->SMBeh->LongVibro();
            SaveHealthState(me->eeprom, DAMAGED);
            break;
        }
        case Q_EXIT_SIG: {
            me->logger->log("Exited state damaged");
            status_ = Q_HANDLED();
            break;
        }
        case RAD_RECEIVED_SIG: {
            me->vars.DecrementCount();
            if (not(me->vars.IsPositive())) {
                status_ = Q_TRAN(&Health_dead);
            } else {
                status_ = Q_HANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Health_global);
            break;
        }
    }
    return status_;
}
QState Health_simple(Health * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->logger->log("Entered state simple");
            // todo add randomizer
            unsigned int new_count = MIN_TIMEOUT_S;
            me->vars.SetCount(new_count);
            me->SMBeh->SetColor(kLightBlue);
            me->SMBeh->LongVibro();
            SaveHealthState(me->eeprom, HEALTH);
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            me->logger->log("Exited state simple");
            status_ = Q_HANDLED();
            break;
        }
        case RAD_RECEIVED_SIG: {
            me->vars.DecrementCount();
            if (not(me->vars.IsPositive())) {
                status_ = Q_TRAN(&Health_damaged);
            } else {
                status_ = Q_HANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Health_global);
            break;
        }
    }
    return status_;
}
/*${SMs::Health::SM::global::dead} .........................................*/
QState Health_dead(Health * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->logger->log("Entered state dead");
            me->SMBeh->SetColor(kLightRed);
            SaveHealthState(me->eeprom, DEAD);
            me->SMBeh->LongVibro();
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            me->logger->log("Exited state dead");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Health_global);
            break;
        }
    }
    return status_;
}

#ifdef DESKTOP
/*${SMs::Health::SM::final} ................................................*/
QState Health_final(Health * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${SMs::Health::SM::final} */
        case Q_ENTRY_SIG: {
            printf("Bye! Bye!");
            exit(0);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
#endif /* def DESKTOP */

/*$enddef${SMs::Health} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/*tranlated from diagrams code*/
