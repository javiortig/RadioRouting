#include "RadioInoStation.h"

RadioInoStation::RadioInoStation(const std::string &id, const StationType &type,
                                 const int &rdPin, const int &tdPin, const int &vccPin)
{
    this->id = id;
    this->type = type;

    this->hc12RDPin = rdPin;
    this->hc12TDPin = tdPin;
    this->hc12VCCPin = vccPin;

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

    while (!flag)
    {
        this->sendMessage(addMessage);
        std::this_thread::sleep_for(std::chrono::seconds(SMALL_NAP_TIME * 2));
        this->buffer = this->readMessage();
        if (this->buffer.hasInstruction(SUCCESSFULLY_ADDED_CMD))
        {
            try
            {
                int ch = std::stoi(this->buffer.getInstruction(SUCCESSFULLY_ADDED_CMD).value);
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
    while (hc12.available())
    {
        hc12.read();
    }
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
