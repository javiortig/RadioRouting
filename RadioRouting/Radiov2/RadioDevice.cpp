#include "RadioDevice.h"

void RadioDevice::sendMessage(std::string messageStr)
{
    this->sendMessage(Message(messageStr));
}

bool RadioDevice::sendMessageWaitForOK(Message message)
{
    this->sendMessage(message);
}

void RadioDevice::blockingWait(float seconds)
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    while (elapsed_seconds.count() <= seconds)
    {
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }
}