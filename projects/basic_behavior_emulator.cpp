#include "stdout_logger.h"
#include "basic_behavior.h"
#include "emulator.h"
#include "fake_led.h"
#include "fake_vibro.h"
#include "fake_beeper.h"

int main() {
    FakeLed led;
    FakeBeeper beeper;
    FakeVibro vibro;
    StdOutLogger logger;
    BasicBehavior b(&logger, &led, &beeper, &vibro);
    Emulator(&b).Run();
}