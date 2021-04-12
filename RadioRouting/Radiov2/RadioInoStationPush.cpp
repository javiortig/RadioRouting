#include "RadioInoStationPush.h"

void RadioInoStationPush::routine()
{
    if (!this->TWH())
    {
        return;
    }

    //If we have instructions to send, send all to router
    if (!this->newInstructions.empty())
    {
        Message data(this->id, this->newInstructions);

        this->sendMessage(data);
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
        this->sendMessage(data);
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
        this->sendMessage(data);
    }
}

void RadioInoStationPush::addInstructionToSend(const Instruction &instruction)
{
    this->newInstructions.push_back(instruction);
}