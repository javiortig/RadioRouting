#pragma once

#include <vector>
#include <iostream>
#include <ctime>

#include "Message.h"

#define MAIN_ROUTER_ID "ROUTER"

#define DEFAULT_CH 0

#define THW_WAIT_TIME 10
#define SMALL_NAP_TIME 1

#define MIN_CH 1
#define MAX_CH 49

/*
    TODO:
    
    -En general, la comunicacion no es muy segura, se puede perder info. Ej:
    una estacion PUSH envia los datos, los da por enviados pero al router le llegan
    con ruido
*/

class RadioDevice
{
protected:
    int currentChannel;

    std::string id; //Has to be unique in the network
    Message buffer;

    int getChannel();
    virtual bool setChannel(int channel) = 0;

    virtual void startHC12() = 0;
    virtual void stopHC12() = 0;
    virtual void writeHC12(const std::string &str) = 0;
    virtual std::string readHC12() = 0;

    virtual bool TWH() = 0; // establish connection

public:
    void sendMessage(std::string messageStr);
    virtual void sendMessage(Message message) = 0;
    virtual Message readMessage() = 0;
};