#include "stdout_logger.h"
#include "basic_behavior.h"
#include "emulator.h"
#include "fake_led.h"

int main() {
    FakeLed led;
    StdOutLogger logger;
    BasicBehavior b(&logger, &led);
    Emulator(&b).Run();
}