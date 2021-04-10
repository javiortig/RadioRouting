#include "RadioPiRouter.h"

RadioPiRouter::RadioPiRouter()
{
    this->id = MAIN_ROUTER_ID;

    //Setup default messages
    this->SYNACK_MSG.instructions.push_back(Instruction(SYNACK_CMD, TWH_DEFAULT_INS_VALUE));
    this->SYNACK_MSG.id = this->id;

    this->FINISH_MSG.instructions.push_back(Instruction(FINISH_CMD, TWH_DEFAULT_INS_VALUE));
    this->FINISH_MSG.id = this->id;
}

void RadioPiRouter::routine()
{
    listenForNewStations();

    //Loop through all stations
    // first is key, second is value
    std::map<int, StationValue>::iterator it;
    for (it = this->stations.begin(); it != stations.end(); it++)
    {
        //TODO: handle if setChannel fails
        this->setChannel(it->first);

        //establish connection
        if (!this->TWH())
            continue;

        //listen to what station has to say
        if (this->listenStation())
        {
            //Write to openHab
        }
        else
        {
            continue;
        }

        //Received and processed, send finish
        this->sendMessage(this->FINISH_MSG);
        sleep(SMALL_NAP_TIME);
        this->sendMessage(this->FINISH_MSG);
    }
}

bool RadioPiRouter::listenForNewStations()
{
}

bool RadioPiRouter::TWH()
{
    bool synFlag = false;
    auto start = std::chrono::system_clock::now();
    do
    {
        this->buffer = this->readMessage();
        // check if received a SYN command and id matches
        if (!this->buffer.isEmpty() && this->buffer.hasInstruction(SYN_CMD))
        {
            //Check that id matches
            if (this->stations[this->getChannel()].id == this->buffer.id)
            {
                synFlag = true;
                break;
            }
        }

        //Check if timeout
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds.count() > THW_WAIT_TIME)
        {
            return false;
        }

        sleep(SMALL_NAP_TIME);
    } while (!synFlag);

    // send SYN ACK
    this->sendMessage(this->SYNACK_MSG);
    sleep(SMALL_NAP_TIME);
    this->sendMessage(this->SYNACK_MSG);

    return true;
}