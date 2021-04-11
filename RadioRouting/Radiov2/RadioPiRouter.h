#pragma once

#include "RadioDevice.h"

#include <unistd.h>
//#include <pigpio>

/*
    TODO:
    - Send messages and wait for OK instead of sending and 'sleeping'

    -A las stations añadir un vector de notificaciones que tiene que decirles
    el router como por ej: oye cambia la hora.

    -Añadir un cronometro a las estaciones y si no se han comunicado en ese tiempo
    borrarlas de la lista

    - Por ahora que sea el ROUTER el que diga cuando acabar las comunicaciones
*/

#define NEW_STATIONS_WAIT_TIME 4

enum StationType
{
    push,
    pull,
    both
};

// The id of the station is the key
struct StationValue
{
    int channel;
    Message lastMessage;
    StationType stationType;
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

    //True if received the message
    bool listenStation();

    //True if Three Way Handshake succeeded
    // The Router should already be in the channel it wants to TWH
    bool TWH();

public:
    RadioPiRouter();

    /*  This is the main function and should be called each iter. of the program loop.
        It requires 2 functions:
        -handleStationInstructions
        -getInstructionsForStation, which should return the instructions to send 
    */
    void routine(void (*handleStationInstructions)(std::string id, std::vector<Instruction> instructions),
                 std::vector<Instruction> (*getInstructionsForStation)(std::string id));
};