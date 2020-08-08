#ifndef LOCKET_API_SEQUENCABLE_H
#define LOCKET_API_SEQUENCABLE_H

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include "api/sequences.h"

template<typename TChunk>
class Sequenceable {
public:
    void StartOrRestart(const TChunk* sequence);
    void StartOrContinue(const TChunk* sequence);
    void Stop();
    virtual ~Sequenceable() { StopThread(); }

protected:
    virtual void Setup(const TChunk& current) = 0;
    virtual void Off() = 0;

private:
    void Actuate();
    void StartThread();
    void StopThread();

    const TChunk* current_sequence = nullptr;
    const TChunk* current_chunk = nullptr;
    std::thread actuation_thread;
    int32_t repeat_counter = 0;
    std::atomic<bool> stopping = false;
};

template<typename TChunk>
void Sequenceable<TChunk>::StartOrRestart(const TChunk* sequence) {
    StopThread();
    current_sequence = sequence;
    current_chunk = sequence;
    repeat_counter = -1;
    StartThread();
}

template<typename TChunk>
void Sequenceable<TChunk>::Actuate() {
    while(!stopping) {   // Process the sequence
        auto delay = std::chrono::milliseconds(0);
        {
            if (!(current_sequence && current_chunk)) continue;
            switch(current_chunk->type) {
                case ChunkType::kSetup: {
                    Setup(*current_chunk);
                    ++current_chunk;
                    break;
                }

                case ChunkType::kWait: {
                    delay = std::chrono::milliseconds(current_chunk->time_ms);
                    ++current_chunk;
                    break;
                }

                case ChunkType::kGoto: {
                    current_chunk = current_sequence + current_chunk->chunk_to_jump_to;
                    delay = std::chrono::milliseconds(1);
                    break;
                }

                case ChunkType::kEnd: {
                    current_sequence = nullptr;
                    current_chunk = nullptr;
                    return;
                }

                case ChunkType::kRepeat:
                    if (repeat_counter == -1) repeat_counter = current_chunk->repeat_count;
                    if (repeat_counter == 0) {   // All was repeated, goto next
                        repeat_counter = -1;     // reset counter
                        current_chunk++;
                    } else {  // repeating in progress
                        current_chunk = current_sequence;  // Always from beginning
                        repeat_counter--;
                    }
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

template<typename TChunk>
void Sequenceable<TChunk>::StartOrContinue(const TChunk* sequence) {
    if (sequence != current_sequence) {
        StartOrRestart(sequence);
    }
}

template<typename TChunk>
void Sequenceable<TChunk>::StartThread() {
    stopping = false;
    actuation_thread = std::thread([&](){Actuate();});
}

template<typename TChunk>
void Sequenceable<TChunk>::StopThread() {
    stopping = true;
    if (actuation_thread.joinable()) {
        actuation_thread.join();
    }
    actuation_thread = std::thread();
}

template<typename TChunk>
void Sequenceable<TChunk>::Stop() {
    StopThread();
    Off();
    current_sequence = nullptr;
    current_chunk = nullptr;
}

#endif //LOCKET_API_SEQUENCABLE_H
