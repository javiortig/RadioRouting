#pragma once

/*
    Library for Rpi and other linux-based systems
*/

#include "Message.h"
#include <thread>
//#include <pigpio.h>
//#include <wiringSerial.h>

#define MAIN_ROUTER_ID "ROUTER"

#define DEFAULT_CH 0

#define THW_WAIT_TIME 10
#define SMALL_NAP_TIME 1

#define MIN_CH 1
#define MAX_CH 49

#define HC12_RD 15
#define HC12_TD 14
#define HC12_VCC 18
#define HC12_SET 23

#define SERIAL_PORT "/dev/ttyS0"

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

    int hc12RDPin;
    int hc12TDPin;
    int hc12VCCPin;
    int hc12SetPin;
    int hc12FileDescriptor;

    int getChannel();
    bool setChannel(int channel);

    // GPIO should be initialized before calling this function. Remember to call
    // gpioTerminate at the end.
    void startHC12();

    void stopHC12();
    void writeHC12(const std::string &str);
    std::string readHC12();

    virtual bool TWH() = 0; // establish connection

public:
    void sendMessage(std::string messageStr);
    void sendMessage(Message message);
    Message readMessage();

    RadioDevice(const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
                const int &vccPin = HC12_VCC, const int &setPin = HC12_SET);
};