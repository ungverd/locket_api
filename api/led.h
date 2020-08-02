#ifndef LOCKET_API_LED_H
#define LOCKET_API_LED_H

struct RgbLedSequence {
    // TODO(aeremin): implement.
};


// Interface allowing to control RGB LED.
class RgbLed {
public:
    virtual void StartOrRestart(const RgbLedSequence& sequence) = 0;
};

#endif //LOCKET_API_LED_H
