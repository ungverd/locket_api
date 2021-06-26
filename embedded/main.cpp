#include "common_states.h"
#include "board.h"
#include "led.h"
#include "vibro.h"
#include "beeper.h"
#include "radio_lvl1.h"
#include "kl_i2c.h"
#include "kl_lib.h"
#include "pill.h"
#include "pill_mgr.h"
#include "MsgQ.h"
#include "SimpleSensors.h"
#include "buttons.h"

// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};

// EEAddresses
#define EE_ADDR_DEVICE_ID   0

int32_t ID;
static uint8_t ISetID(int32_t NewID);
void ReadIDfromEE();

// ==== Periphery ====
Vibro_t VibroMotor {VIBRO_SETUP};
Beeper_t Beeper {BEEPER_PIN};

LedRGBwPower_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN, LED_EN_PIN };

enum RCmd_t : uint8_t {
    rcmdNone = 0,
    rcmdPing = 1,
    rcmdPong = 2,
    rcmdBeacon = 3,
    rcmdScream = 4,
    rcmdLustraParams = 5,
    rcmdLocketSetParam = 6,
    rcmdLocketGetParam = 7,
    rcmdLocketExplode = 8,
    rcmdLocketDieAll = 9,
    rcmdLocketDieChoosen = 10,
};

struct rPkt_t {
    uint16_t From;  // 2
    uint16_t To;    // 2
    uint16_t TransmitterID; // 2
    RCmd_t Cmd; // 1
    uint8_t id; // 1
};

RadioLevel1<rPkt_t> Radio;

// ==== Timers ====
static TmrKL_t TmrEverySecond {TIME_MS2I(1000), evtIdEverySecond, tktPeriodic};

static void CheckRxData();
[[noreturn]] static void ITask();
static void OnCmd(Shell_t *PShell);

int main(void) {
}

#define ID_MIN                  1
#define ID_MAX                  140
#define ID_DEFAULT              ID_MIN


uint8_t ISetID(int32_t NewID) {
    if(NewID < ID_MIN or NewID > ID_MAX) return retvFail;
    uint8_t rslt = EE::Write32(EE_ADDR_DEVICE_ID, NewID);
    if(rslt == retvOk) {
        ID = NewID;
        Printf("New ID: %u\r", ID);
        return retvOk;
    }
    else {
        Printf("EE error: %u\r", rslt);
        return retvFail;
    }
}

