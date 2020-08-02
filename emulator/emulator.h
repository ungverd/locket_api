#ifndef LOCKET_API_EMULATOR_EMULATOR_H
#define LOCKET_API_EMULATOR_EMULATOR_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include <chrono>
#include <thread>
#include <mutex>

#include "behavior.h"

template <typename PillState = EmptyState, typename RadioPacket = EmptyState>
class Emulator {
public:
    Emulator(Behavior<PillState, RadioPacket>* behavior): behavior(behavior) {};

    void Run();

private:
    std::mutex behavior_mutex;
    Behavior<PillState, RadioPacket>* const behavior;
};

template<typename PillState, typename RadioPacket>
void Emulator<PillState, RadioPacket>::Run() {
    behavior->OnStarted();
    std::thread every_second_timer([&](){
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::lock_guard l(behavior_mutex);
            behavior->EverySecond();
        }
    });

    while(true){}
}

#pragma clang diagnostic pop
#endif //LOCKET_API_EMULATOR_EMULATOR_H
