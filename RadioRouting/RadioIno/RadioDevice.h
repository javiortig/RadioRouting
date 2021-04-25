// #pragma once

// #include "Message.h"

// #define MAIN_ROUTER_ID "ROUTER"

// #define DEFAULT_CH 1

// #define THW_WAIT_TIME 10
// #define SMALL_NAP_TIME 1
// #define HC12_DELAY_M 1000

// #define MIN_CH 2
// #define MAX_CH 49

// #define HC12_RD 4
// #define HC12_TD 3
// #define HC12_VCC 2
// #define HC12_SET 5

// #define SET_CHANNEL_STR "AT+C0"

// /*
//     TODO:

//     -En general, la comunicacion no es muy segura, se puede perder info. Ej:
//     una estacion PUSH envia los datos, los da por enviados pero al router le llegan
//     con ruido
// */

// class RadioDevice
// {
// protected:
//     SoftwareSerial *hc12;

//     unsigned char hc12RDPin;
//     unsigned char hc12TDPin;
//     unsigned char hc12VCCPin;
//     unsigned char hc12SetPin;

//     unsigned char currentChannel;

//     char id[MAX_ID_SIZE]; //Has to be unique in the network
//     Message buffer;

//     unsigned char getChannel();
//     bool setChannel(const unsigned char &channel);

//     void startHC12();
//     void stopHC12();
//     void writeHC12(const char *str);
//     void readHC12();

//     virtual bool TWH() = 0; // establish connection

// public:
//     void sendMessage(String messageStr);
//     void sendMessage(Message message);
//     Message readMessage();

//     //RadioDevice(const int &rdPin, const int &tdPin,
//     //                           const int &vccPin, const int &setPin);

//     //~RadioDevice();
// };
