#ifndef LOCKET_API_RADIO_WRAPPER_H
#define LOCKET_API_RADIO_WRAPPER_H

#include "Radio/radio_lvl1.h"
#include "api/radio.h"

template <typename TRadioPacket>
class RadioWrapper: public Radio<TRadioPacket> {
public:
    explicit RadioWrapper(rLevel1_t* radio): radio(radio) {}
    void Transmit(const TRadioPacket& packet) override {
        packet_to_send = packet;
        radio->PktTx = &packet_to_send;
        radio->PktTxSize = sizeof(TRadioPacket);
        RMsg_t msg;
        msg.Cmd = R_MSG_TRANSMIT;
        radio->RMsgQ.SendNowOrExit(msg);
    }

private:
    rLevel1_t* radio;
    TRadioPacket packet_to_send;
};

#endif //LOCKET_API_RADIO_WRAPPER_H
