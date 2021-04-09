#pragma once

#include <vector>
#include <iostream>
#include <ctime>

#include "Message.h"

#define MAIN_ROUTER_ID "ROUTER"

#define DEFAULT_CH 0

class RadioDevice
{
protected:
    int currentChannel;
    std::string id; //Has to be unique in the network
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