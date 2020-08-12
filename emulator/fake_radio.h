#ifndef LOCKET_API_FAKE_RADIO_H
#define LOCKET_API_FAKE_RADIO_H

#include "api/radio.h"

template <typename TRadioPacket>
class FakeRadio: public Radio<TRadioPacket> {
public:
    void Transmit(const TRadioPacket& packet) override {}
};

#endif //LOCKET_API_FAKE_RADIO_H
