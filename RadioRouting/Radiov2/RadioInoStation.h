#pragma once

#include "RadioDevice.h"
//#include <SoftwareSerial.h>

#define HC12_RD 4
#define HC12_TD 3
#define HC12_VCC 2

class RadioInoStation : public RadioDevice
{
private:
    int hc12RDPin;
    int hc12TDPin;
    int hc12VCCPin;

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
                    const int &rdPin, const int &tdPin, const int &vccPin);

    // Requests the router to be added to the network.
    // This function will not return untill successfully added
    void requestAdd();

    void sendMessage(Message message) override;
    Message readMessage() override;
};