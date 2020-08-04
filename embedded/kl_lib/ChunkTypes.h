/*
 * ChunkTypes.h
 *
 *  Created on: 08 ���. 2015 �.
 *      Author: Kreyl
 */

#pragma once

#include <sequences.h>
#include "color.h"
#include "ch.h"
#include "MsgQ.h"
#include "sequences_collection.h"

#if 1 // ====================== Base sequencer class ===========================
enum SequencerLoopTask_t {sltProceed, sltBreak};

template <class TChunk>
class BaseSequencer_t : private IrqHandler_t {
protected:
    virtual_timer_t ITmr;
    const TChunk *IPStartChunk, *IPCurrentChunk;
    int32_t RepeatCounter = -1;
    EvtMsg_t IEvtMsg;
    virtual void ISwitchOff() = 0;
    virtual SequencerLoopTask_t ISetup() = 0;
    void SetupDelay(uint32_t ms) { chVTSetI(&ITmr, TIME_MS2I(ms), TmrKLCallback, this); }

    // Process sequence
    void IIrqHandler() {
        if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);  // Reset timer
        while(true) {   // Process the sequence
            switch(IPCurrentChunk->type) {
                case ChunkType::kSetup: // setup now and exit if required
                    if(ISetup() == sltBreak) return;
                    break;

                case ChunkType::kWait: { // Start timer, pointing to next chunk
                        uint32_t Delay = IPCurrentChunk->time_ms;
                        IPCurrentChunk++;
                        if(Delay != 0) {
                            SetupDelay(Delay);
                            return;
                        }
                    }
                    break;

                case ChunkType::kGoto:
                    IPCurrentChunk = IPStartChunk + IPCurrentChunk->chunk_to_jump_to;
                    if(IEvtMsg.ID != evtIdNone) EvtQMain.SendNowOrExitI(IEvtMsg);
                    SetupDelay(1);
                    return;
                    break;

                case ChunkType::kEnd:
                    if(IEvtMsg.ID != evtIdNone) EvtQMain.SendNowOrExitI(IEvtMsg);
                    IPStartChunk = nullptr;
                    IPCurrentChunk = nullptr;
                    return;
                    break;

                case ChunkType::kRepeat:
                    if(RepeatCounter == -1) RepeatCounter = IPCurrentChunk->repeat_count;
                    if(RepeatCounter == 0) {    // All was repeated, goto next
                        RepeatCounter = -1;     // reset counter
                        IPCurrentChunk++;
                    }
                    else {  // repeating in progress
                        IPCurrentChunk = IPStartChunk;  // Always from beginning
                        RepeatCounter--;
                    }
                    break;
            } // switch
        } // while
    } // IProcessSequenceI
public:
    void SetupSeqEndEvt(EvtMsg_t AEvtMsg) { IEvtMsg = AEvtMsg; }

    void StartOrRestart(const TChunk *PChunk) {
        chSysLock();
        RepeatCounter = -1;
        IPStartChunk = PChunk;   // Save first chunk
        IPCurrentChunk = PChunk;
        IIrqHandler();
        chSysUnlock();
    }

    void StartOrContinue(const TChunk *PChunk) {
        if(PChunk == IPStartChunk) return; // Same sequence
        else StartOrRestart(PChunk);
    }

    void Stop() {
        if(IPStartChunk != nullptr) {
            chSysLock();
            if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
            IPStartChunk = nullptr;
            IPCurrentChunk = nullptr;
            chSysUnlock();
        }
        ISwitchOff();
    }
    const TChunk* GetCurrentSequence() { return IPStartChunk; }
    bool IsIdle() { return (IPStartChunk == nullptr and IPCurrentChunk == nullptr); }
};
#endif
