#include "RadioDevice.h"

bool RadioDevice::setChannel(const unsigned char &channel)
{
    char smsg[8];
    strcpy(smsg, SET_CHANNEL_STR);

    if ((channel < MIN_CH || channel > MAX_CH) && channel != DEFAULT_CH)
        return false;
    if (channel < 10)
    {
        smsg[5] = '0';
        smsg[6] = channel + '0';
    }
    else
    {
        smsg[5] = '0' + (channel / 10);
        smsg[6] = '0' + (channel % 10);
    }

    digitalWrite(this->hc12SetPin, LOW);
    delay(300);

    this->writeHC12(smsg);

    if (this->hc12->read() != 'O')
    {
        digitalWrite(this->hc12SetPin, HIGH);

        Serial.print(F("Error in setChannel\n"));

        delay(100);
        return false;
    }

    digitalWrite(this->hc12SetPin, HIGH);

    Serial.print(F("Channel changed successfully\n"));

    delay(120);
    this->currentChannel = channel;
    return true;
}
