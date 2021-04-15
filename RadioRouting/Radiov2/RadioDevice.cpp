#include "RadioDevice.h"

void RadioDevice::sendMessage(std::string messageStr)
{
    this->sendMessage(Message(messageStr));
}

RadioDevice::RadioDevice(const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
                         const int &vccPin = HC12_VCC, const int &setPin = HC12_SET)
{
    this->hc12RDPin = rdPin;
    this->hc12TDPin = tdPin;
    this->hc12VCCPin = vccPin;
    this->hc12SetPin = setPin;
}

bool RadioDevice::setChannel(int channel)
{
    std::string smsg = "AT+C0";
    std::string temp = "";

    //clear hc12
    serialFlush(this->hc12FileDescriptor);

    if (channel < MIN_CH || channel > MAX_CH)
        return false;
    if (channel < 10)
    {
        smsg += '0';
    }
    smsg += std::to_string(channel);

    gpioWrite(this->hc12SetPin, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    this->writeHC12(smsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    temp = this->readHC12();

    if (temp != smsg)
    {
        gpioWrite(this->hc12SetPin, 1);
        return false;
    }

    gpioWrite(this->hc12SetPin, 1);
    this->currentChannel = channel;
    return true;
}

void RadioDevice::startHC12()
{
    gpioSetMode(this->hc12VCCPin, PI_OUTPUT);
    gpioWrite(this->hc12VCCPin, 1);

    gpioSetMode(this->hc12SetPin, PI_OUTPUT);
    gpioWrite(this->hc12SetPin, 1);

    this->hc12FileDescriptor = serialOpen(SERIAL_PORT, 9600);
    //Error opening serial
    if (this->hc12FileDescriptor == -1)
    {
        return;
    }
}

void RadioDevice::stopHC12()
{
    serialClose(this->hc12FileDescriptor);
    gpioWrite(this->hc12VCCPin, 1);
}

void RadioDevice::writeHC12(const std::string &str)
{
    serialFlush(this->hc12FileDescriptor);

    serialPrintf(this->hc12FileDescriptor, str);
}
std::string RadioDevice::readHC12()
{
    std::string b = "";
    while (serialDataAvail(this->hc12FileDescriptor) > 0)
    {
        b += serialGetchar(this->hc12FileDescriptor);
    }

    return b;
}

Message RadioDevice::readMessage()
{
    return Message::strToMessage(this->readHC12());
}

void RadioDevice::sendMessage(Message message)
{
    this->writeHC12(Message::messageToString(message));
}
