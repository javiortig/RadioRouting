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

void RadioPiRouter::routine(void (*handleStationInstructions)(std::string id, std::vector<Instruction> instructions),
                            std::vector<Instruction> (*getInstructionsForStation)(std::string id))
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

        std::vector<Instruction> instructionsToSend;
        switch (it->second.stationType)
        {
        case StationType::push:
            //listen to what station has to say
            if (this->listenStation())
            {
                //Write to openHab
                handleStationInstructions(it->second.id,
                                          it->second.lastMessage.instructions);
            }
            break;
        case StationType::pull:
            //Send data to them(if we have to)
            instructionsToSend = getInstructionsForStation(it->second.id);

            // We have new instructions, so we send.
            if (!instructionsToSend.empty())
            {
                this->sendMessage(Message(this->id, instructionsToSend));
                this->blockingWait(SMALL_NAP_TIME);
                this->sendMessage(Message(this->id, instructionsToSend));
                this->blockingWait(SMALL_NAP_TIME);
            }
            break;
        case StationType::both:
            //TODO: push & pull

            break;

        default:
            break;
        }

        //Received and processed, send finish
        this->sendMessage(this->FINISH_MSG);
        this->blockingWait(SMALL_NAP_TIME);
        this->sendMessage(this->FINISH_MSG);
    }
}

bool RadioPiRouter::listenForNewStations()
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    while (elapsed_seconds.count() <= NEW_STATIONS_WAIT_TIME)
    {
        this->buffer = this->readMessage();
        // check if received a NEW command
        if (!this->buffer.isEmpty() && this->buffer.hasInstruction(NEW_CMD))
        {
            //Check that the id hasn't been taken. If taken, kick old and let the new in
            std::map<int, StationValue>::iterator it;
            for (it = this->stations.begin(); it != stations.end();)
            {
                if (it->second.id == buffer.id)
                    stations.erase(it++);
                else
                    ++it;
            }

            //TODO: assign new channel and create station
        }
    }
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

        this->blockingWait(SMALL_NAP_TIME);
    } while (!synFlag);

    // send SYN ACK
    this->sendMessage(this->SYNACK_MSG);
    this->blockingWait(SMALL_NAP_TIME);
    this->sendMessage(this->SYNACK_MSG);

    return true;
}