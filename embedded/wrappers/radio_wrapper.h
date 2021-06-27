#ifndef LOCKET_API_RADIO_WRAPPER_H
#define LOCKET_API_RADIO_WRAPPER_H

#include "Radio/radio_lvl1.h"
#include "Radio/radio_lvl2.h"
#include "api/radio.h"

template <typename TRadioPacket>
class RadioWrapperSimple: public Radio<TRadioPacket> {
public:
    explicit RadioWrapperSimple(RadioLevel1<TRadioPacket>* radio): radio(radio) {}
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

    TRadioPacket FetchReceived() override {
        return radio->received_packets.Fetch(TIME_IMMEDIATE);
    }

private:
    RadioLevel1<TRadioPacket>* radio;
    TRadioPacket packet_to_send_once, packet_beacon;
};

template <typename TRadioPacket>
class RadioWrapperManyToMany: public Radio<TRadioPacket> {
public:
    explicit RadioWrapperManyToMany(RadioLevel2<TRadioPacket>* radio): radio(radio) {}
    void Transmit(const TRadioPacket& packet) override {
        radio->TransmitOnce(packet);
    }

    void SetBeaconPacket(const TRadioPacket& packet) override {
        radio->SetBeaconPacketTo(packet);
    }

    void ClearBeaconPacket() override {
        radio->ClearBeaconPacket();
    }

    void SetPowerLevel(RadioPowerLevel level) override {
        // TODO(aeremin) Implement.
        Printf("SetPowerLevel is not yet supported for many-to-many radio\n");
    }

    TRadioPacket FetchReceived() override {
        return radio->FetchReceivedPacket();
    }

private:
    RadioLevel2<TRadioPacket>* radio;
};

#endif //LOCKET_API_RADIO_WRAPPER_H
