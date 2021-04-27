#include "RadioInoStation.h"

// /*
// RadioInoStation::RadioInoStation(const String &id, const StationType &type,
//                                  const int &rdPin, const int &tdPin,
//                                  const int &vccPin, const int &setPin): RadioDevice(rdPin, tdPin, vccPin, setPin)
// {
//     this->id = id;
//     this->type = type;

//     // Setup default messages
//     this->SYN_MSG.instructions.push_back(Instruction(SYN_CMD, TWH_DEFAULT_INS_VALUE));
//     this->SYN_MSG.id = this->id;

//     // Request the addition to the Router
//     this->requestAdd();
// }
// */
bool RadioInoStation::TWH()
{
    //send SYN command
    unsigned long start = millis();
    unsigned long fin;

    Serial.println("Entering TWH");

    do
    {
        fin = millis();

        this->buffer.createDefault(this->id);
        this->buffer.insertInstruction(SYN_CMD, EMPTY_INSTRUCTION_VALUE);
        this->sendMessage();
        delay(SMALL_NAP_TIME * 2 * 1000);

        //check if synack received and id matches
        if (this->readMessage() && this->buffer.findInstruction(SYNACK_CMD))
        {
            //Check that id matches
            if (this->buffer.compareId(MAIN_ROUTER_ID))
            {
                Serial.print("TWH Successfull\n");
                return true;
            }
        }

    } while (fin - start <= (unsigned long)THW_WAIT_TIME * 1000 * 4);

    Serial.print("TWH failed\n");
    return false;
}

void RadioInoStation::requestAdd(const char *type)
{
    Serial.print(F("Requesting add...\n"));
    bool flag = false;

    //Change to default channel. Blocking untill successfully changed channel
    while (!this->setChannel(DEFAULT_CH))
        ;

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    //{000;id}

    while (!flag)
    {
        digitalWrite(LED_BUILTIN, HIGH);

        this->buffer.createDefault(this->id);
        this->buffer.insertInstruction(NEW_CMD, type);
        this->sendMessage();

        if (!this->readMessage())
            continue;

        Serial.print("loop\n");

        if (this->buffer.findInstruction(SUCCESSFULLY_ADDED_CMD))
        {
            //Extract the channel and assign
            unsigned char ch, i;
            for (ch = 0, i = 0; buffer.eIndex >= buffer.sIndex; --buffer.eIndex, ++i)
            {
                ch += (*buffer.eIndex - '0') * pow(10, i);
            }

            if (ch <= 0 || ch > MAX_CH)
            {
                // Invalid conversion
                flag = false;
                continue;
            }

            //will be true only if successfully change channel
            while (!this->setChannel(ch))
                ;
            flag = true;
        }

        digitalWrite(LED_BUILTIN, LOW);
        delay(SMALL_NAP_TIME * 1000);
    }

    Serial.print("Successfully added to the network by router\n");

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
}
