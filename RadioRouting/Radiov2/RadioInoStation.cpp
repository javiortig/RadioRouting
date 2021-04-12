#include "RadioInoStation.h"

RadioInoStation::RadioInoStation(const std::string &id, const StationType &type)
{
    this->id = id;
    this->type = type;

    //Setup default messages
    this->SYN_MSG.instructions.push_back(Instruction(SYN_CMD, TWH_DEFAULT_INS_VALUE));
    this->SYN_MSG.id = this->id;
}

bool RadioInoStation::TWH()
{
    //send SYN command
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    while (elapsed_seconds.count() <= THW_WAIT_TIME * 4)
    {

        this->sendMessage(this->SYN_MSG);
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME * 2));

        //check if synack received and id matches
        this->buffer = this->readMessage();
        if (!this->buffer.isEmpty() && this->buffer.hasInstruction(SYNACK_CMD))
        {
            //Check that id matches
            if (this->buffer.id == MAIN_ROUTER_ID)
            {
                return true;
            }
        }

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }

    return false;
}