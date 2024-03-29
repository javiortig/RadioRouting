#pragma once

#include "RadioDevice.h"

#include <unistd.h>
#include <map>
#include <set>

/*
    TODO:
    - Send messages and wait for OK instead of sending and 'sleeping'

    -A las stations añadir un vector de notificaciones que tiene que decirles
    el router como por ej: oye cambia la hora.

    -Añadir un cronometro a las estaciones y si no se han comunicado en ese tiempo
    borrarlas de la lista

    - Por ahora que sea el ROUTER el que diga cuando acabar las comunicaciones
*/

#define NEW_STATIONS_TIMEOUT 4
#define LISTEN_STATION_TIMEOUT 7

// The id of the station is the key
struct StationValue
{
    int channel;
    Message lastMessage;
    StationType stationType;

    StationValue(int channel, StationType type)
    {
        this->channel = channel;
        this->stationType = type;
    }

    StationValue()
    {
	this->channel = -1;
	this->stationType = StationType::undefined;
    }
};

class RadioPiRouter : public RadioDevice
{
private:
    // save this msg since its going to be sent very often
    Message SYNACK_MSG;
    Message FINISH_MSG;

    //The channel of the device is the key
    std::map<std::string, StationValue> stations;

    // True if found a new one
    bool listenForNewStations();

    //True if received the message. listens for the current channel
    bool listenStation();

    //True if Three Way Handshake succeeded
    // The Router should already be in the channel it wants to TWH
    bool TWH() override;

    // Returns the channel where the station was added. <0 if not succeeded
    int addStation(const std::string &id, const StationType &type);

public:
    RadioPiRouter(const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
                  const int &vccPin = HC12_VCC, const int &setPin = HC12_SET);

    /*  This is the main function and should be called each iter. of the program loop.
        It requires 2 functions:
        -handleStationInstructions
        -getInstructionsForStation, which should return the instructions to send 
         or empty vector if no new instructions
    */
    void routine(void (*handleStationInstructions)(std::string id, std::vector<Instruction> instructions),
                 std::vector<Instruction> (*getInstructionsForStation)(std::string id));
};
