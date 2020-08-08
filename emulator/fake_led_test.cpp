#include <gtest/gtest.h>
#include "fake_led.h"

TEST(FakeLed, CanOverrideCurrentSequence) {
    FakeLed led;
    led.StartOrRestart(kStartSequence);
    led.StartOrRestart(kFailureSequence);
}

