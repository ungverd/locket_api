#include "try_giganda.h"
#include "emulator.h"
#include "fake_led.h"
#include "stdout_logger.h"

int main() {
    FakeLed led;
    StdOutLogger logger;
    GigandaBehavior b(&logger, &led);
    Emulator(&b).Run();
}