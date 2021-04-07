#pragma once

#include "RadioDevice.h"

//#include <pigpio>

struct Station
{
    int channel;
    Message lastMessage;
};

class RadioPiRouter : public RadioDevice
{
private:
    std::vector<Station> stations;

    bool _listenForNewStations();

public:
    RadioPiRouter();
    ~RadioPiRouter();

    void routine();
};