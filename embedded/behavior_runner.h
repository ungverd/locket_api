#ifndef LOCKET_API_BEHAVIOR_RUNNER_H
#define LOCKET_API_BEHAVIOR_RUNNER_H

#include "board.h"

#include "embedded/wrappers/beeper_wrapper.h"
#include "embedded/wrappers/vibro_wrapper.h"
#include "embedded/wrappers/rgb_led_wrapper.h"
#include "embedded/wrappers/logger_wrapper.h"
#include "embedded/wrappers/pill_manager_wrapper.h"
#include "embedded/wrappers/radio_wrapper.h"
#include "embedded/kl_lib/led.h"
#include "embedded/kl_lib/vibro.h"
#include "embedded/kl_lib/beeper.h"
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
VibroWrapper vibroWrapper(&VibroMotor);

Beeper_t Beeper {BEEPER_PIN};
BeeperWrapper beeperWrapper(&Beeper);

LedRGBwPower_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN, LED_EN_PIN };
RgbLedWrapper ledWrapper(&Led);
LoggerWrapper loggerWrapper(&Uart);


// ==== Timers ====
TmrKL_t TmrEverySecond {TIME_MS2I(1000), evtIdEverySecond, tktPeriodic};

uint8_t ReadDipSwitch();

namespace embedded {
// BehaviorType must be subtype of Behavior
template<typename BehaviorType>
class BehaviorRunner {
private:
    Behavior<typename BehaviorType::PillStateParameter,
            typename BehaviorType::RadioPacketParameter>* behavior = nullptr;
    PillManagerWrapper<typename BehaviorType::PillStateParameter> pill_manager_wrapper{&PillMgr};
public:
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
        g_radio_singleton.Init();

        RadioWrapper<typename BehaviorType::RadioPacketParameter> radioWrapper;

        behavior = new BehaviorType(&loggerWrapper, &ledWrapper, &beeperWrapper, &vibroWrapper, &radioWrapper);
        CheckDipSwitch();
        behavior->OnStarted();

        while (true) {
            EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
            switch (Msg.ID) {
                case evtIdEverySecond:
                    PillMgr.Check();
                    CheckDipSwitch();
                    behavior->EverySecond();
                    break;
                case evtIdButtons:
                    // TODO(aeremin) Support other button events - long presses, double-clicks, combos, etc.
                    if (Msg.BtnEvtInfo.Type == beShortPress) {
                        behavior->OnButtonPressed(Msg.BtnEvtInfo.BtnID[0]);
                    }
                    break;

                case evtIdPillConnected:
                    behavior->OnPillConnected(&pill_manager_wrapper);
                    break;

                case evtIdPillDisconnected:
                    behavior->OnPillDisconnected();
                    break;
            }
        }
    }

    void CheckDipSwitch() {
        static uint32_t previous_dip_value = 0xFFFF;
        uint8_t current_dip_value = ReadDipSwitch();
        if (current_dip_value != previous_dip_value) {
            behavior->OnDipSwitchChanged(current_dip_value);
            previous_dip_value = current_dip_value;
        }
    }
};

}

uint8_t ReadDipSwitch() {
    const PinInputSetup_t dip_switch_pins[DIP_SW_CNT] =
            {DIP_SW1, DIP_SW2, DIP_SW3, DIP_SW4, DIP_SW5, DIP_SW6, DIP_SW7, DIP_SW8 };
    uint8_t result = 0;
    for(int i=0; i<DIP_SW_CNT; i++) PinSetupInput(dip_switch_pins[i].PGpio,
                                                  dip_switch_pins[i].Pin,
                                                  dip_switch_pins[i].PullUpDown);
    for(int i=0; i<DIP_SW_CNT; i++) {
        if(!PinIsHi(dip_switch_pins[i].PGpio, dip_switch_pins[i].Pin)) result |= (1 << i);
        PinSetupAnalog(dip_switch_pins[i].PGpio, dip_switch_pins[i].Pin);
    }
    return result;
}

#endif //LOCKET_API_BEHAVIOR_RUNNER_H
