/*
 * radio_lvl1.h
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#pragma once

#include "kl_lib.h"
#include "ch.h"
#include "cc1101.h"
#include "kl_buf.h"
#include "uart.h"
#include "MsgQ.h"

// Message queue
#define R_MSGQ_LEN      4 // Length of q
#define R_MSG_SET_PWR   1
#define R_MSG_SET_CHNL  2
#define R_MSG_TRANSMIT  4
struct RMsg_t {
    uint8_t Cmd;
    uint8_t Value;
    RMsg_t() : Cmd(0), Value(0) {}
    RMsg_t(uint8_t ACmd) : Cmd(ACmd), Value(0) {}
    RMsg_t(uint8_t ACmd, uint8_t AValue) : Cmd(ACmd), Value(AValue) {}
} __attribute__((packed));

#define RCHNL_MIN       10
#define ID2RCHNL(ID)    (RCHNL_MIN + ID)

// Timings
#define MIN_SLEEP_DURATION_MS   18

template <typename TRadioPacket>
class RadioLevel1 {
public:
    EvtMsgQ_t<RMsg_t, R_MSGQ_LEN> RMsgQ;
    EvtMsgQ_t<TRadioPacket, R_MSGQ_LEN> received_packets;
    TRadioPacket* PktTxOnce = nullptr;
    TRadioPacket* PktTxBeacon = nullptr;
    uint8_t Init();
    // Inner use
    void TryToSleep(uint32_t SleepDuration);
};

template <typename TRadioPacket>
[[noreturn]]
static void rLvl1Thread(void* arg) {
    auto* radio_instance = static_cast<RadioLevel1<TRadioPacket>*>(arg);
    chRegSetThreadName("rLvl1");
    while(true) {
        // ==== TX if needed ====
        RMsg_t Msg = radio_instance->RMsgQ.Fetch(TIME_IMMEDIATE);
        if(Msg.Cmd == R_MSG_TRANSMIT) {
            CC.SetTxPower(CC_PwrMinus20dBm);
            for(int i=0; i<4; i++) {
                CC.Recalibrate();
                CC.Transmit(radio_instance->PktTxOnce, sizeof(TRadioPacket));
                chThdSleepMilliseconds(99);
            }
        }

        if (radio_instance->PktTxBeacon) {
            CC.SetTxPower(CC_PwrMinus20dBm);
            CC.Recalibrate();
            CC.Transmit(radio_instance->PktTxBeacon, sizeof(TRadioPacket));
        }

        // ==== Rx ====
        CC.Recalibrate();

        int8_t Rssi;
        TRadioPacket PktRx;
        if(CC.Receive(360, &PktRx, sizeof(TRadioPacket), &Rssi) == retvOk) {
            Printf("Received radio packet");
            EvtQMain.SendNowOrExit({evtIdRadioCmd});
            radio_instance->received_packets.SendNowOrExit(PktRx);
        }

        radio_instance->TryToSleep(300);
    } // while true
}

template <typename TRadioPacket>
void RadioLevel1<TRadioPacket>::TryToSleep(uint32_t SleepDuration) {
    if(SleepDuration >= MIN_SLEEP_DURATION_MS) CC.EnterPwrDown();
    chThdSleepMilliseconds(SleepDuration);
}

template <typename TRadioPacket>
uint8_t RadioLevel1<TRadioPacket>::Init() {
    static THD_WORKING_AREA(warLvl1Thread, 256);

    received_packets.Init();
    RMsgQ.Init();
    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_PwrMinus20dBm);
        CC.SetPktSize(sizeof(TRadioPacket));
        CC.SetChannel(1);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread<TRadioPacket>, this);
        return retvOk;
    }
    else return retvFail;
}
