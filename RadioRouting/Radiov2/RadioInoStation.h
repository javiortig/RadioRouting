#pragma once

#include "RadioDevice.h"
#include <SoftwareSerial.h>

#define HC12_RD 4
#define HC12_TD 3
#define HC12_VCC 2
#define HC12_SET 5

class RadioInoStation : public RadioDevice
{
private:
    int hc12RDPin;
    int hc12TDPin;
    int hc12VCCPin;
    int hc12SetPin;

    SoftwareSerial hc12;

    bool setChannel(int channel) override;

    void startHC12() override;
    void stopHC12() override;
    void writeHC12(const std::string &str) override;
    std::string readHC12() override;

protected:
    StationType type;

    Message SYN_MSG;

    bool TWH();

public:
    RadioInoStation(const std::string &id, const StationType &type,
                    const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
                    const int &vccPin = HC12_VCC, const int &setPin = HC12_SET);

    // Requests the router to be added to the network.
    // This function will not return untill successfully added(blocking)
    void requestAdd();

    void sendMessage(Message message) override;
    Message readMessage() override;
};