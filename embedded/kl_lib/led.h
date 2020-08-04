/*
 * led_rgb.h
 *
 *  Created on: 31 ���. 2014 �.
 *      Author: Kreyl
 */

#pragma once

#include "hal.h"
#include "color.h"
#include "ChunkTypes.h"
#include "uart.h"
#include "kl_lib.h"

class LedRGBParent_t : public BaseSequencer_t<LedRGBChunk> {
protected:
    const PinOutputPWM_t  R, G, B;
    const uint32_t PWMFreq;
    Color ICurrColor;
    void ISwitchOff() {
        SetColor(kBlack);
        ICurrColor = kBlack;
    }
    SequencerLoopTask_t ISetup() {
        if(ICurrColor != IPCurrentChunk->color) {
            if(IPCurrentChunk->value == 0) {     // If smooth time is zero,
                SetColor(IPCurrentChunk->color); // set color now,
                ICurrColor = IPCurrentChunk->color;
                IPCurrentChunk++;                // and goto next chunk
            }
            else {
                ICurrColor.Adjust(IPCurrentChunk->color);
                SetColor(ICurrColor);
                // Check if completed now
                if(ICurrColor == IPCurrentChunk->color) IPCurrentChunk++;
                else { // Not completed
                    // Calculate time to next adjustment
                    uint32_t Delay = ICurrColor.DelayToNextAdjustment(IPCurrentChunk->color, IPCurrentChunk->value);
                    SetupDelay(Delay);
                    return sltBreak;
                } // Not completed
            } // if time > 256
        } // if color is different
        else IPCurrentChunk++; // Color is the same, goto next chunk
        return sltProceed;
    }
public:
    LedRGBParent_t(
            const PwmSetup_t ARed,
            const PwmSetup_t AGreen,
            const PwmSetup_t ABlue,
            const uint32_t APWMFreq) :
        BaseSequencer_t(), R(ARed), G(AGreen), B(ABlue), PWMFreq(APWMFreq) {}
    void Init() {
        R.Init();
        R.SetFrequencyHz(PWMFreq);
        G.Init();
        G.SetFrequencyHz(PWMFreq);
        B.Init();
        B.SetFrequencyHz(PWMFreq);
        SetColor(kBlack);
    }
    bool IsOff() { return (ICurrColor == kBlack) and IsIdle(); }
    virtual void SetColor(Color AColor) {}
};

class LedRGB_t : public LedRGBParent_t {
public:
    LedRGB_t(
            const PwmSetup_t ARed,
            const PwmSetup_t AGreen,
            const PwmSetup_t ABlue,
            const uint32_t AFreq = 0xFFFFFFFF) :
                LedRGBParent_t(ARed, AGreen, ABlue, AFreq) {}

    void SetColor(Color AColor) {
        R.Set(AColor.r);
        G.Set(AColor.g);
        B.Set(AColor.b);
    }
};

class LedRGBwPower_t : public LedRGBParent_t {
private:
    const PinOutput_t PwrPin;
public:
    LedRGBwPower_t(
            const PwmSetup_t ARed,
            const PwmSetup_t AGreen,
            const PwmSetup_t ABlue,
            const PinOutput_t APwrPin,
            const uint32_t AFreq = 0xFFFFFFFF) :
                LedRGBParent_t(ARed, AGreen, ABlue, AFreq), PwrPin(APwrPin) {}
    void Init() {
        PwrPin.Init();
        LedRGBParent_t::Init();
    }
    void SetColor(Color AColor) {
        if(AColor == kBlack) PwrPin.SetLo();
        else PwrPin.SetHi();
        R.Set(AColor.r);
        G.Set(AColor.g);
        B.Set(AColor.b);
    }
};

