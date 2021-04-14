#include "RadioInoStation.h"

RadioInoStation::RadioInoStation(const String &id, const StationType &type,
                                 const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
                                 const int &vccPin = HC12_VCC, const int &setPin = HC12_SET)
{
    this->id = id;
    this->type = type;

    this->hc12RDPin = rdPin;
    this->hc12TDPin = tdPin;
    this->hc12VCCPin = vccPin;
    this->hc12SetPin = setPin;

    // Setup default messages
    this->SYN_MSG.instructions.push_back(Instruction(SYN_CMD, TWH_DEFAULT_INS_VALUE));
    this->SYN_MSG.id = this->id;

    // Request the addition to the Router
    this->requestAdd();
}

bool RadioInoStation::TWH()
{
    //send SYN command
    ulong start = millis();
    ulong end = millis();

    do
    {

        this->sendMessage(this->SYN_MSG);
        delay(SMALL_NAP_TIME * 2);

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
    } while (end - start <= THW_WAIT_TIME * 1000 * 4);

    return false;
}

void RadioInoStation::requestAdd()
{
    Message addMessage(this->id, Instruction(NEW_CMD, this->type));
    bool flag = false;

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    while (!flag)
    {
        this->sendMessage(addMessage);

        digitalWrite(LED_BUILTIN, HIGH);

        delay(SMALL_NAP_TIME * 2);

        digitalWrite(LED_BUILTIN, LOW);

        this->buffer = this->readMessage();
        if (this->buffer.hasInstruction(SUCCESSFULLY_ADDED_CMD))
        {

            int ch = this->buffer.getInstruction(SUCCESSFULLY_ADDED_CMD).value.toInt();

            if (ch == 0)
            {
                // Invalid conversion
                flag = false;
                continue;
            }

            //will be tru only if successfully change channel
            flag = this->setChannel(ch);
        }
    }
}

bool RadioInoStation::setChannel(int channel)
{
    String smsg = "AT+C0";
    String temp = "";

    //clear hc12
    while (hc12.available())
    {
        hc12.read();
    }

    if (channel < MIN_CH || channel > MAX_CH)
        return false;
    if (channel < 10)
    {
        smsg += '0';
    }
    smsg += String(channel);

    digitalWrite(this->hc12SetPin, LOW);
    delay(60);
    hc12.print(smsg);
    delay(60);
    temp = this->readHC12();

    if (temp != smsg)
    {
        digitalWrite(set, HIGH);
        delay(60);
        return false;
    }

    digitalWrite(set, HIGH);
    delay(80);
    this->currentChannel = channel;
    return true;
}

void RadioInoStation::startHC12()
{
    pinMode(this->hc12VCCPin, OUTPUT);
    digitalWrite(this->hc12VCCPin, HIGH);
    delay(50);
    this->hc12 = SoftwareSerial(this->hc12RDPin, this->hc12TDPin);
    delay(50);
    this->hc12.begin(9600);
}

void RadioInoStation::stopHC12()
{
    this->hc12.end();
    delay(50);

    digitalWrite(this->hc12VCCPin, LOW);
}

void RadioInoStation::writeHC12(const String &str)
{
    while (hc12.available())
    {
        hc12.read();
    }

    hc12.print(str);
}
String RadioInoStation::readHC12()
{
    String b = "";
    while (hc12.available())
    {
        b += hc12.read();
    }

    return b;
}

Message RadioInoStation::readMessage()
{
    return Message::strToMessage(this->readHC12());
}

void RadioInoStation::sendMessage(Message message)
{
    while (hc12.available())
    {
        hc12.read();
    }

    hc12.print(Message::messageToString(message));
}
