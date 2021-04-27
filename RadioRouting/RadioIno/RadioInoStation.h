#pragma once

#include "RadioDevice.h"

class RadioInoStation : public RadioDevice
{
protected:
    Message SYN_MSG;

    bool TWH();

public:
    // Requests the router to be added to the network.
    // This function will not return untill successfully added(blocking)
    void requestAdd(const char *type);
};
