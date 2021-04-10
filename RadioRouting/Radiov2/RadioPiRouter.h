#pragma once

#include "RadioDevice.h"

#include <unistd.h>
//#include <pigpio>

#define THW_WAIT_TIME 10
#define SMALL_NAP_TIME 1

/*
    TODO:
    -A las stations añadir un vector de notificaciones que tiene que decirles
    el router como por ej: oye cambia la hora.

    -Añadir un cronometro a las estaciones y si no se han comunicado en ese tiempo
    borrarlas de la lista
*/
// The channel of the station is the key
struct StationValue
{
    std::string id;
    Message lastMessage;
};

class RadioPiRouter : public RadioDevice
{
private:
    // WE WILL save this msg since its going to be sent very often
    Message SYNACK_MSG;
    Message FINISH_MSG;

    //The channel of the device is the key
    std::map<int, StationValue> stations;

    // True if found a new one
    bool listenForNewStations();

    //True if received the message
    bool listenStation();

    //True if Three Way Handshake succeeded
    bool TWH();

public:
    RadioPiRouter();

    void routine();
};