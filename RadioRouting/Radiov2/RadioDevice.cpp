#include "RadioDevice.h"

void RadioDevice::sendMessage(std::string messageStr)
{
    this->sendMessage(Message(messageStr));
}