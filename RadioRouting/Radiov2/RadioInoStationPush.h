#pragma once

#include "RadioInoStation.h"

class RadioInoStationPush : public RadioInoStation
{
private:
    // This vector will delete the instruction each msgTime the instruction is sent
    std::vector<Instruction> newInstructions;

public:
    // Returns true if successfully contacted with the Router
    void routine();

    //called by user before routine to send a message
    void addInstructionToSend(const Instruction &instruction);
};