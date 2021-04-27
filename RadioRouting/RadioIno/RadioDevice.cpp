#include "RadioDevice.h"

unsigned char RadioDevice::getChannel()
{
    return this->currentChannel;
}

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

void RadioDevice::startHC12()
{
    pinMode(this->hc12VCCPin, OUTPUT);
    digitalWrite(this->hc12VCCPin, HIGH);
    delay(100);

    pinMode(this->hc12SetPin, OUTPUT);
    digitalWrite(this->hc12SetPin, HIGH);
    delay(100);

    this->hc12 = new SoftwareSerial(this->hc12RDPin, this->hc12TDPin);
    delay(200);
    this->hc12->begin(9600);
    delay(100);
}

void RadioDevice::stopHC12()
{
    this->hc12->end();
    delay(200);
    delete this->hc12;

    digitalWrite(this->hc12VCCPin, LOW);
    digitalWrite(this->hc12SetPin, HIGH);
    delay(100);
}

void RadioDevice::writeHC12(const char *str)
{
    //clear hc12
    while (hc12->available())
    {
        hc12->read();
    }

    hc12->print(str);
    delay(HC12_DELAY_M);
}

bool RadioDevice::readHC12()
{
    delay(HC12_DELAY_M);

    this->buffer.sIndex = this->buffer.value;
    while (hc12->available() && (this->buffer.sIndex < this->buffer.value + MSG_SIZE))
    {
        *this->buffer.sIndex += char(hc12->read());

        ++this->buffer.sIndex;
    }

    return this->buffer.validateMessage();
}

void RadioDevice::sendMessage()
{
    this->writeHC12(this->buffer.value);
}

bool RadioDevice::readMessage()
{
    return this->readHC12();
}