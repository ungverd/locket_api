#ifndef LOCKET_API_RADIO_H
#define LOCKET_API_RADIO_H

template <typename TRadioPacketType>
class Radio {
public:
    // Transmits provided packet "once" (technically, it can do bunch of retries, but it's guaranteed that all
    // retries will happen "very fast" (in less than a second).
    virtual void Transmit(const TRadioPacketType& packet) = 0;

    // Sets the packet which will be transmitted regularly (bunch of times per second).
    virtual void SetBeaconPacket(const TRadioPacketType& packet) = 0;

    // Un-does SetBeaconPacket - stops regular sending.
    virtual void ClearBeaconPacket() = 0;

    virtual ~Radio() {};
};

#endif //LOCKET_API_RADIO_H
