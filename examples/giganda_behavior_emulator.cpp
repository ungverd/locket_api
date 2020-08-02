#include "try_giganda.h"
#include "emulator.h"
#include "fake_led.h"

int main() {
    FakeLed led;
    GigandaBehavior b(&led);
    Emulator(&b).Run();
}