#pragma once

#include "RadioDevice.h"

class RadioInoStation : public RadioDevice
{
protected:
    StationType type;

    Message SYN_MSG;

    bool TWH();

public:
    /*RadioInoStation(const String &id, const StationType &type,
                    const int &rdPin, const int &tdPin,
                    const int &vccPin, const int &setPin);
    */

    // Requests the router to be added to the network.
    // This function will not return untill successfully added(blocking)
    void requestAdd();
};
