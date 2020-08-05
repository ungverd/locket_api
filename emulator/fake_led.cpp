#include "fake_led.h"
#include <chrono>
#include <iostream>

void FakeLed::StartOrRestart(const LedRGBChunk* sequence) {
    std::lock_guard l(sequence_mutex);
    current_sequence = sequence;
    current_chunk = sequence;
    repeat_counter = -1;
    actuation_thread = std::thread([&](){Actuate();});
}

void FakeLed::Actuate() {
    while(true) {   // Process the sequence
        auto delay = std::chrono::milliseconds(0);
        {
            std::lock_guard l(sequence_mutex);
            switch(current_chunk->type) {
                case ChunkType::kSetup: {
                    if (current_chunk->color == kBlack) {
                        std::cout << "Turning LED off" << std::endl;
                    } else {
                        std::cout << "Setting LED to " <<
                                  uint32_t(current_chunk->color.r) << " " <<
                                  uint32_t(current_chunk->color.g) << " " <<
                                  uint32_t(current_chunk->color.b) << std::endl;
                    }
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
