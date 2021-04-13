#include "RadioInoStation.h"

RadioInoStation::RadioInoStation(const std::string &id, const StationType &type,
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
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    while (elapsed_seconds.count() <= THW_WAIT_TIME * 4)
    {

        this->sendMessage(this->SYN_MSG);
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME * 2));

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

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }

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

        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME * 2));

        digitalWrite(LED_BUILTIN, LOW);

        this->buffer = this->readMessage();
        if (this->buffer.hasInstruction(SUCCESSFULLY_ADDED_CMD))
        {
            try
            {
                int ch = std::stoi(this->buffer.getInstruction(SUCCESSFULLY_ADDED_CMD).value);

                //will be tru only if successfully change channel
                flag = this->setChannel(ch);
            }
            catch (const std::invalid_argument &e)
            {
                // Invalid conversion
                flag = false;
                continue;
            }
        }
    }
}

bool RadioInoStation::setChannel(int channel)
{
    std::string smsg = "AT+C0";
    std::string temp = "";

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
    smsg += std::to_string(channel);

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

void RadioInoStation::writeHC12(const std::string &str)
{
    while (hc12.available())
    {
        hc12.read();
    }

    hc12.print(str);
}
std::string RadioInoStation::readHC12()
{
    std::string b = "";
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
