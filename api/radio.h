#ifndef LOCKET_API_RADIO_H
#define LOCKET_API_RADIO_H

#include <utility>

enum class RadioPowerLevel {
    MINUS_30_DBM = 0x03,
    MINUS_27_DBM = 0x08,
    MINUS_25_DBM = 0x0D,
    MINUS_20_DBM = 0x17,
    MINUS_15_DBM = 0x1D,
    MINUS_10_DBM = 0x26,
    MINUS_6_DBM = 0x37,
    PLUS_0_DBM = 0x50,
    PLUS_5_DBM = 0x86,
    PLUS_7_DBM = 0xCD,
    PLUS_10_DBM = 0xC5,
    PLUS_12_DBM = 0xC0,
};

template<typename TRadioPacketType>
class Radio {
public:
    // Transmits provided packet "once" (technically, it can do bunch of retries, but it's guaranteed that all
    // retries will happen "very fast" (in less than a second).
    virtual void Transmit(const TRadioPacketType& packet) = 0;

    // Sets the packet which will be transmitted regularly (bunch of times per second).
    virtual void SetBeaconPacket(const TRadioPacketType& packet) = 0;

    // Un-does SetBeaconPacket - stops regular sending.
    virtual void ClearBeaconPacket() = 0;

    virtual void SetPowerLevel(RadioPowerLevel level) = 0;

    // Internal, don't use directly.
    virtual std::pair<TRadioPacketType, int8_t> FetchReceived() = 0;

    virtual ~Radio() {};
};

#endif //LOCKET_API_RADIO_H
