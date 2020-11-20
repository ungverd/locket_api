#ifndef LOCKET_API_RADIO_WRAPPER_H
#define LOCKET_API_RADIO_WRAPPER_H

#include "Radio/radio_lvl1.h"
#include "api/radio.h"

template <typename TRadioPacket>
class RadioWrapper: public Radio<TRadioPacket> {
public:
    explicit RadioWrapper(RadioLevel1<TRadioPacket>* radio): radio(radio) {}
    void Transmit(const TRadioPacket& packet) override {
        packet_to_send_once = packet;
        radio->PktTxOnce = &packet_to_send_once;
        RMsg_t msg;
        msg.Cmd = R_MSG_TRANSMIT;
        radio->RMsgQ.SendNowOrExit(msg);
    }

    void SetBeaconPacket(const TRadioPacket& packet) override {
        packet_beacon = packet;
        radio->PktTxBeacon = &packet_beacon;
    }

    void ClearBeaconPacket() override {
        radio->PktTxBeacon = nullptr;
    }

    void SetPowerLevel(RadioPowerLevel level) override {
        RMsg_t msg;
        msg.Cmd = R_MSG_SET_PWR;
        msg.Value = static_cast<uint8_t>(level);
        radio->RMsgQ.SendNowOrExit(msg);
    }

private:
    RadioLevel1<TRadioPacket>* radio;
    TRadioPacket packet_to_send_once, packet_beacon;
};

#endif //LOCKET_API_RADIO_WRAPPER_H
