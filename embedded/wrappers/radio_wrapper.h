#ifndef LOCKET_API_RADIO_WRAPPER_H
#define LOCKET_API_RADIO_WRAPPER_H

#include "api/radio.h"

template <typename TRadioPacket>
class RadioWrapper: public Radio<TRadioPacket> {
public:
    void Transmit(const TRadioPacket& packet) override {}
};

#endif //LOCKET_API_RADIO_WRAPPER_H
