#pragma once

#include "RadioDevice.h"

#include <unistd.h>
//#include <pigpio>

#define THW_WAIT_TIME 10
#define SMALL_NAP_TIME 1

struct StationValue
{
    std::string id;
    Message lastMessage;
};

class RadioPiRouter : public RadioDevice
{
private:
    //The channel of the device is the key
    std::map<int, StationValue> stations;

    bool _listenForNewStations();

    bool _TWH();

public:
    RadioPiRouter();

    void routine();
};