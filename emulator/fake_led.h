#ifndef LOCKET_API_EMULATOR_FAKE_LED_H
#define LOCKET_API_EMULATOR_FAKE_LED_H

#include <mutex>
#include <thread>
#include "led.h"

class FakeLed: public RgbLed {
public:
    ~FakeLed() { Stop(); }
    void StartOrRestart(const LedRGBChunk* sequence) override;
    void Stop();

private:
    void Actuate();
    void StartThread();
    void StopThread();

    std::mutex sequence_mutex; // Guards current_sequence & current_chunk
    const LedRGBChunk* current_sequence = nullptr;
    const LedRGBChunk* current_chunk = nullptr;
    std::thread actuation_thread;
    int32_t repeat_counter = 0;
    bool stopping = false;
};

#endif //LOCKET_API_EMULATOR_FAKE_LED_H
