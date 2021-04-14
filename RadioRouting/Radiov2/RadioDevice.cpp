#include "RadioDevice.h"

void RadioDevice::sendMessage(String messageStr)
{
    this->sendMessage(Message(messageStr));
}
