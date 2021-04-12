#pragma once

#include "RadioInoStation.h"

class RadioInoStationPush : public RadioInoStation
{
private:
    // This vector will delete the instruction each time the instruction is sent
    std::vector<Instruction> newInstructions;

public:
    void routine();
};