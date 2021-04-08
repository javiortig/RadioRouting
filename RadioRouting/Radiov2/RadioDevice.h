#pragma once

#include <vector>
#include <iostream>
#include <ctime>

#include "Message.h"

#define DEFAULT_CH 0

class RadioDevice
{
protected:
    int channel;
    Message readBuffer;

    virtual void _startHC12() = 0;
    virtual void _stopHC12() = 0;
    virtual void _writeHC12() = 0;
    virtual void _readHC12() = 0;

public:
    virtual void sendMessage(Message message) = 0;
    virtual Message readMessage() = 0;
};