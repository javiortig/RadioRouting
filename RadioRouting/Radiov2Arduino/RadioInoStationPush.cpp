#include "RadioInoStationPush.h"

void RadioInoStationPush::routine()
{
    //If we have instructions to send, send all to router
    if (!this->newInstructions.empty())
    {
        if (!this->TWH())
        {
            return;
        }

        Message data(this->id, this->newInstructions);

        this->sendMessage(data);
        delay(SMALL_NAP_TIME * 1000);
        this->sendMessage(data);
        delay(SMALL_NAP_TIME * 1000);
        this->sendMessage(data);
    }
}

void RadioInoStationPush::addInstructionToSend(const Instruction &instruction)
{
    this->newInstructions.push_back(instruction);
}