#pragma once

#include "RadioDevice.h"

class RadioInoStation : public RadioDevice
{
protected:
    StationType type;

    Message SYN_MSG;

    bool TWH();

public:
    RadioInoStation(const std::string &id, const StationType &type);
};