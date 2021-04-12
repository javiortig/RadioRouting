#include "RadioDevice.h"

void RadioDevice::sendMessage(std::string messageStr)
{
    this->sendMessage(Message(messageStr));
}

bool RadioDevice::sendMessageWaitForOK(Message message)
{
    this->sendMessage(message);
}

StationType RadioDevice::strToStationType(const std::string &str)
{
    if (str == NEW_INS_VALUE_PUSH)
        return StationType::push;
    else if (str == NEW_INS_VALUE_PULL)
        return StationType::pull;
    else if (str == NEW_INS_VALUE_BOTH)
        return StationType::both;
    else
        return StationType::undefined;
}