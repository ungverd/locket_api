#ifndef LOCKET_API_FAKE_RADIO_H
#define LOCKET_API_FAKE_RADIO_H

#include "api/radio.h"

template <typename TRadioPacket>
class FakeRadio: public Radio<TRadioPacket> {
public:
    void Transmit(const TRadioPacket& packet) override {}
    void SetBeaconPacket(const TRadioPacket& packet) override {}
    void ClearBeaconPacket() override {}
    void SetPowerLevel(RadioPowerLevel level) override {}
    std::pair<TRadioPacket, int8_t> FetchReceived() override { return {{}, 0}; }
};

#endif //LOCKET_API_FAKE_RADIO_H
