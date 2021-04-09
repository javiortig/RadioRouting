#pragma once

#include <vector>
#include <iostream>
#include <ctime>

#include "Message.h"

#define MAIN_ROUTER_ID 0
#define DEFAULT_CH 0

class RadioDevice
{
protected:
    int currentChannel;
    int id;
    Message buffer;

    virtual void _startHC12() = 0;
    virtual void _stopHC12() = 0;
    virtual void _writeHC12() = 0;
    virtual void _readHC12() = 0;

    virtual bool _TWH() = 0; // establish connection

public:
    void sendMessage(std::string messageStr);
    virtual void sendMessage(Message message) = 0;
    virtual Message readMessage() = 0;
};