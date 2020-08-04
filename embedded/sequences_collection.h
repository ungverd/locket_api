/*
 * Sequences.h
 *
 *  Created on: 09 ���. 2015 �.
 *      Author: Kreyl
 */

#pragma once

#include "ChunkTypes.h"

const LedRGBChunk lsqStart[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kGreen},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, kBlue},
        {ChunkType::kWait, 207},
        {ChunkType::kSetup, 0, {0,4,0}},
        {ChunkType::kEnd},
};

const LedRGBChunk lsqFailure[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd}
};

const LedRGBChunk lsqSearch[] = {
        {ChunkType::kSetup, 0, kRed},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0, kBlack},
        {ChunkType::kEnd},
};


#if 1 // ============================= Beeper ==================================
#define BEEP_VOLUME     2   // Maximum 10

#if 1 // ==== Notes ====
#define La_2    880

#define Do_3    1047
#define Do_D_3  1109
#define Re_3    1175
#define Re_D_3  1245
#define Mi_3    1319
#define Fa_3    1397
#define Fa_D_3  1480
#define Sol_3   1568
#define Sol_D_3 1661
#define La_3    1720
#define Si_B_3  1865
#define Si_3    1976

#define Do_4    2093
#define Do_D_4  2217
#define Re_4    2349
#define Re_D_4  2489
#define Mi_4    2637
#define Fa_4    2794
#define Fa_D_4  2960
#define Sol_4   3136
#define Sol_D_4 3332
#define La_4    3440
#define Si_B_4  3729
#define Si_4    3951

// Length
#define OneSixteenth    90
#define OneEighth       (OneSixteenth * 2)
#define OneFourth       (OneSixteenth * 4)
#define OneHalfth       (OneSixteenth * 8)
#define OneWhole        (OneSixteenth * 16)
#endif

// Type, BEEP_VOLUME, freq
const BeepChunk bsqOn[] = {
        {ChunkType::kSetup, 10, 7000},
        {ChunkType::kEnd}
};

const BeepChunk bsqButton[] = {
        {ChunkType::kSetup, 1, 1975},
        {ChunkType::kWait, 54},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};
const BeepChunk bsqBeepBeep[] = {
        {ChunkType::kSetup, BEEP_VOLUME, 1975},
        {ChunkType::kWait, 54},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 54},
        {ChunkType::kSetup, BEEP_VOLUME, 1975},
        {ChunkType::kWait, 54},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};

const BeepChunk bsqSearch[] = {
        {ChunkType::kSetup, BEEP_VOLUME, 1975},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};

#if 1 // ==== Extensions ====
// Pill
const BeepChunk bsqBeepPillOk[] = {
        {ChunkType::kSetup, BEEP_VOLUME, Si_3},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, BEEP_VOLUME, Re_D_4},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, BEEP_VOLUME, Fa_D_4},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};

const BeepChunk bsqBeepPillBad[] = {
        {ChunkType::kSetup, BEEP_VOLUME, Fa_4},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, BEEP_VOLUME, Re_4},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, BEEP_VOLUME, Si_3},
        {ChunkType::kWait, 180},
        {ChunkType::kSetup, 0},
        {ChunkType::kEnd}
};
#endif // ext
#endif // beeper

#if 1 // ============================== Vibro ==================================
#define VIBRO_VOLUME    100  // 1 to 100

#define VIBRO_SHORT_MS          99
#define VIBRO_REPEAT_PERIOD     1008

const BaseChunk vsqBrr[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, VIBRO_REPEAT_PERIOD},
        {ChunkType::kEnd}
};

const BaseChunk vsqBrrBrr[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, VIBRO_REPEAT_PERIOD},
        {ChunkType::kEnd}
};

const BaseChunk vsqBrrBrrBrr[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, VIBRO_REPEAT_PERIOD},
        {ChunkType::kEnd}
};

// ==== Health ====
const BaseChunk vsqIll[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, 999},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 3600},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kGoto, 2}
};

const BaseChunk vsqDeath[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, 999},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 4005},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kGoto, 2}
};

// Cataclysm
const BaseChunk vsqCataclysm[] = {
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, 999},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 999},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kSetup, 0},
        {ChunkType::kWait, 99},
        {ChunkType::kSetup, VIBRO_VOLUME},
        {ChunkType::kWait, VIBRO_SHORT_MS},
        {ChunkType::kGoto, 2}
};

#endif
