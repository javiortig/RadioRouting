#include "RadioPiRouter.h"

RadioPiRouter::RadioPiRouter()
{
    this->id = MAIN_ROUTER_ID;
}

void RadioPiRouter::routine()
{
    _listenForNewStations();
}

bool RadioPiRouter::_listenForNewStations()
{
}

bool RadioPiRouter::_TWH()
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
            if (this->stations[this->currentChannel].id == this->buffer.id)
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
    // sendMessage(MSG_START_C + "15")
}

// {15;ROUTER;SAK}