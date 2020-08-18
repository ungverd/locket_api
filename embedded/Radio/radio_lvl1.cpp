/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "uart.h"

rLevel1_t g_radio_singleton;

static THD_WORKING_AREA(warLvl1Thread, 256);

template <typename TRadioPacket>
[[noreturn]]
static void rLvl1Thread(void* arg) {
    auto* radio_instance = static_cast<rLevel1_t*>(arg);
    chRegSetThreadName("rLvl1");
    while(true) {
        // ==== TX if needed ====
        RMsg_t Msg = radio_instance->RMsgQ.Fetch(TIME_IMMEDIATE);
        if(Msg.Cmd == R_MSG_TRANSMIT) {
            CC.SetTxPower(CC_PwrMinus20dBm);
            for(int i=0; i<4; i++) {
                CC.Recalibrate();
                CC.Transmit(radio_instance->PktTx, radio_instance->PktTxSize);
                chThdSleepMilliseconds(99);
            }
        }
        // ==== Rx ====
        CC.Recalibrate();

        int8_t Rssi;
        rPkt_t PktRx;
        if(CC.Receive(360, &PktRx, RPKT_LEN, &Rssi) == retvOk) {
            Printf("From: %u; To: %u; TrrID: %u; PktID: %u; Cmd: %u; Rssi: %d\r\n", PktRx.From, PktRx.To, PktRx.TransmitterID, PktRx.PktID, PktRx.Cmd, Rssi);
            switch(PktRx.Cmd) {
                case rcmdLocketDieAll:
                    EvtQMain.SendNowOrExit(EvtMsg_t(evtIdShinePktMutant));
                    break;

                default: break;
            }
        }
    } // while true
}

void rLevel1_t::TryToSleep(uint32_t SleepDuration) {
    if(SleepDuration >= MIN_SLEEP_DURATION_MS) CC.EnterPwrDown();
    chThdSleepMilliseconds(SleepDuration);
}

uint8_t rLevel1_t::Init() {
    RMsgQ.Init();
    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_PwrMinus20dBm);
        CC.SetPktSize(RPKT_LEN);
        CC.SetChannel(1);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread<rPkt_t>, this);
        return retvOk;
    }
    else return retvFail;
}
