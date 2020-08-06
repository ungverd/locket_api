#ifndef LOCKET_API_BEHAVIOR_RUNNER_H
#define LOCKET_API_BEHAVIOR_RUNNER_H

#include <embedded/wrappers/logger_wrapper.h>
#include "board.h"

#include "embedded/wrappers/rgb_led_wrapper.h"
#include "embedded/kl_lib/led.h"
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

const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
int32_t ID;
// ==== Periphery ====
Vibro_t VibroMotor {VIBRO_SETUP};
Beeper_t Beeper {BEEPER_PIN};
LedRGBwPower_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN, LED_EN_PIN };
RgbLedWrapper ledWrapper(&Led);
LoggerWrapper loggerWrapper;

// ==== Timers ====
TmrKL_t TmrEverySecond {TIME_MS2I(1000), evtIdEverySecond, tktPeriodic};

template<typename BehaviorType>
[[noreturn]] void Run() {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V5);
    Clk.SetMSI4MHz();
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit(Clk.APB1FreqHz);
    chSysInit();
    EvtQMain.Init();

    // ==== Init hardware ====
    Uart.Init();
    //ReadIDfromEE();
    //Printf("\r%S %S; ID=%u\r", APP_NAME, XSTRINGIFY(BUILD_TIME), ID);
    Clk.PrintFreqs();

    Led.Init();
    VibroMotor.Init();
    Beeper.Init();
    Beeper.StartOrRestart(bsqBeepBeep);
    SimpleSensors::Init();

    i2c1.Init();
    PillMgr.Init();

    // ==== Time and timers ====
    TmrEverySecond.StartOrRestart();

    // ==== Radio ====
    Radio.Init();

    BehaviorType behavior(&loggerWrapper, &ledWrapper);
    behavior.OnStarted();

    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdEverySecond:
                PillMgr.Check();
                behavior.EverySecond();
                break;
        }
    }
}

#endif //LOCKET_API_BEHAVIOR_RUNNER_H
