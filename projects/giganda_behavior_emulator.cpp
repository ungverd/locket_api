#include "giganda_behavior.h"
#include "emulator.h"
#include "stdout_logger.h"
#include "fake_led.h"
#include "fake_vibro.h"
#include "fake_beeper.h"

int main() {
    FakeLed led;
    FakeBeeper beeper;
    FakeVibro vibro;
    StdOutLogger logger;
    GigandaBehavior b(&logger, &led, &beeper, &vibro);
    Emulator(&b).Run();
}

