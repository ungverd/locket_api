#ifndef LOCKET_API_RADIO_H
#define LOCKET_API_RADIO_H

template <typename TRadioPacketType>
class Radio {
public:
    virtual void Transmit(const TRadioPacketType& packet) = 0;
    virtual ~Radio() {};
};

#endif //LOCKET_API_RADIO_H
