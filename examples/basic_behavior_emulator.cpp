#include "basic_behavior.h"
#include "emulator.h"
#include "fake_led.h"

int main() {
    FakeLed led;
    BasicBehavior b(&led);
    Emulator(&b).Run();
}