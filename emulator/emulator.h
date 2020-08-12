#ifndef LOCKET_API_EMULATOR_EMULATOR_H
#define LOCKET_API_EMULATOR_EMULATOR_H

#include <atomic>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <iostream>

#include "behavior.h"
#include "fake_pill_manager.h"
#include "fake_led.h"
#include "fake_beeper.h"
#include "fake_vibro.h"
#include "stdout_logger.h"
#include "fake_radio.h"

namespace emulator {

template<typename BehaviorType>
class BehaviorRunner {
public:
    void Run() {
        FakeLed led;
        FakeBeeper beeper;
        FakeVibro vibro;
        FakeRadio<typename BehaviorType::RadioPacketParameter> radio;
        StdOutLogger logger;
        behavior = std::make_unique<BehaviorType>(&logger, &led, &beeper, &vibro, &radio);
        std::atomic<bool> stopping_execution = false;

        std::cout << "Execution started. Supported commands: \n";
        std::cout << "  General commands: \n";
        std::cout << "    quit - stops emulator \n";
        std::cout << "  Pill commands: \n";
        std::cout << "    connect_pill - Simulates pill being inserted \n";
        std::cout << "    remove_pill - Simulates pill being removed \n";
        std::cout << "    write_pill <number 0-255> - Sets the pill ID to provided number \n";
        std::cout << "  Button commands: \n";
        std::cout << "    button <number 0-2> - Simulates pressing corresponding button \n";
        std::cout << "  DIP switch commands: \n";
        std::cout
                << "    dip <number 1-8> <number 0-1> - Simulates setting switch on position specified by first argument \n";
        std::cout << "                                    to state specified by the second argument \n";

        behavior->OnDipSwitchChanged(dip_value);
        behavior->OnStarted();
        std::thread every_second_timer([&]() {
            while (!stopping_execution) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::lock_guard l(behavior_mutex);
                behavior->EverySecond();
            }
        });

        std::string user_input;
        while (user_input != "q" && user_input != "quit") {
            std::getline(std::cin, user_input);
            std::stringstream input(user_input);
            std::string command;
            input >> command;
            std::lock_guard l(behavior_mutex);
            if (command == "connect_pill") {
                behavior->OnPillConnected(&pill_manager);
            } else if (command == "disconnect_pill") {
                behavior->OnPillDisconnected();
            } else if (command == "write_pill") {
                int32_t value;
                input >> value;
                if (value < 0 || value > 255) {
                    std::cout << "Pill value must be in range 0-255!" << std::endl;
                    continue;
                }

                pill_manager.WritePill({static_cast<uint32_t>(value)});
                std::cout << "Written " << value << " to pill" << std::endl;
            } else if (command == "button") {
                int32_t position;
                input >> position;
                if (position < 0 || position > 2) {
                    std::cout << "Button index must be in range 0-2!" << std::endl;
                    continue;
                }
                behavior->OnButtonPressed(position);
            } else if (command == "dip") {
                int32_t position, on;
                input >> position >> on;
                if (position < 1 || position > 8) {
                    std::cout << "DIP position must be in range 1-8!" << std::endl;
                    continue;
                }
                --position;
                dip_value = dip_value ^ (dip_value & (1 << position));
                if (on) dip_value = dip_value | (1 << position);
                behavior->OnDipSwitchChanged(dip_value);
            }
        }
        stopping_execution = true;
        every_second_timer.join();
    }

private:
    std::unique_ptr<Behavior<typename BehaviorType::PillStateParameter,
                             typename BehaviorType::RadioPacketParameter>> behavior;
    std::mutex behavior_mutex;
    FakePillManager pill_manager;
    uint8_t dip_value = 0;
};

} // namespace emulator

#endif //LOCKET_API_EMULATOR_EMULATOR_H
