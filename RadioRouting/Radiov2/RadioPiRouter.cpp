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

bool RadioPiRouter::listenStation()
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    while (elapsed_seconds.count() <= LISTEN_STATION_TIMEOUT)
    {
        this->buffer = this->readMessage();
        // Check id matches
        if (!this->buffer.isEmpty() &&
            this->stations[this->buffer.id].channel == this->getChannel())
        {
            //TODO: hacer que esto lo haga todo la funcion readMessage()
            this->stations[this->buffer.id].lastMessage = this->buffer;
            return true;
        }

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }

    return false;
}

void RadioPiRouter::routine(void (*handleStationInstructions)(std::string id, std::vector<Instruction> instructions),
                            std::vector<Instruction> (*getInstructionsForStation)(std::string id))
{
    listenForNewStations();

    //Loop through all stations
    // first is key, second is value
    for (auto it = this->stations.begin(); it != stations.end(); it++)
    {
        //TODO: handle if setChannel fails
        this->setChannel(it->second.channel);

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
                handleStationInstructions(it->first,
                                          it->second.lastMessage.instructions);
            }
            break;
        case StationType::pull:
            //Send data to them(if we have to)
            instructionsToSend = getInstructionsForStation(it->first);

            // We have new instructions, so we send.
            if (!instructionsToSend.empty())
            {
                this->sendMessage(Message(this->id, instructionsToSend));
                std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
                this->sendMessage(Message(this->id, instructionsToSend));
                std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
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
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
        this->sendMessage(this->FINISH_MSG);
    }
}

bool RadioPiRouter::listenForNewStations()
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    while (elapsed_seconds.count() <= NEW_STATIONS_TIMEOUT)
    {
        this->buffer = this->readMessage();
        // check if received a NEW command
        if (!this->buffer.isEmpty() && this->buffer.hasInstruction(NEW_CMD))
        {
            this->addStation(this->buffer.id,
                             RadioDevice::strToStationType(this->buffer.getInstruction(NEW_CMD).value));

            return true;
        }

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }

    return false;
}

bool RadioPiRouter::addStation(const std::string &id, const StationType &type)
{
    //Check that the id hasn't been taken. If taken, kick old and let the new in
    for (auto it = this->stations.begin(); it != stations.end();)
    {
        if (it->first == id)
            this->stations.erase(it++);
        else
            ++it;
    }

    //Adds a new station with a unique channel
    std::set<int> uniqueChannels;
    for (auto it = this->stations.begin(); it != stations.end(); it++)
    {
        uniqueChannels.insert(it->second.channel);
    }

    //try all numbers in the channel range:
    for (int i = MIN_CH; i < MAX_CH; i++)
    {
        if (uniqueChannels.insert(i).second)
        {
            //channel is free. proceed to insertion
            this->stations[id] = StationValue(i, type);
            return true;
        }
    }

    return false;
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
            if (this->stations[this->buffer.id].channel == this->getChannel())
            {
                this->stations[this->buffer.id].lastMessage = this->buffer;
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

        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
    } while (!synFlag);

    // send SYN ACK
    this->sendMessage(this->SYNACK_MSG);
    std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME));
    this->sendMessage(this->SYNACK_MSG);

    return true;
}