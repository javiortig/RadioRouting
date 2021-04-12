#pragma once

#include "RadioInoStation.h"

class RadioInoStationPush : public RadioInoStation
{
private:
    // This vector will delete the instruction each time the instruction is sent
    std::vector<Instruction> newInstructions;

public:
    // Returns true if successfully contacted with the Router
    void routine();

    void addInstructionToSend(const Instruction &instruction);
};