#include "Message.h"

Message::Message(const String &id, const std::vector<Instruction> &instructions)
{
    this->instructions = instructions;
    this->id = id;
}

Message::Message(const String &id, const Instruction &singleInstruction)
{
    this->instructions.push_back(singleInstruction);
    this->id = id;
}

Message::Message(const String &messageStr)
{
    *this = Message::strToMessage(messageStr);
}

std::vector<String> Message::_chopString(String str, char delim)
{
    std::vector<String> cont;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos)
    {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));

    return cont;
}

bool Message::isEmpty()
{
    return this->instructions.empty();
}

Message Message::strToMessage(String str)
{
    Message result;
    int sPos = str.find(MSG_START_C);
    int ePos = str.find(MSG_END_C);
    // Invalid message. return empty
    if (sPos < 0 || ePos < 0 || str.length() < 3 || sPos + 1 >= str.length() || !isdigit(str[sPos + 1]))
    {
#ifdef DEBUG
        std::cout << "Error: str input\n";
#endif
        return Message();
    }
    str = str.substr(sPos + 1, ePos - sPos - 1);
    std::vector<String> v = _chopString(str, MSG_SEP_C);

#ifdef DEBUG
    std::cout << "Full msg string: " << str << " with len: " << str.length() + 2 << std::endl;
    for (size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << std::endl;

    std::cout << "--end chop--\n";
#endif

    if (v.size() <= 2)
    {
#ifdef DEBUG
        std::cout << "Error: msg empty\n";
#endif
        return Message(); // Message is Empty
    }

    //extract number and check if it matches len
    int msgLen = stoi(v[0]);

    if (msgLen != str.length() + 2)
    {
#ifdef DEBUG
        std::cout << "Error: msg len doesnt match with len:" << msgLen
                  << " and str.length(): " << str.length() << std::endl;
#endif
        return Message();
    }

    //extract id:
    result.id = v[1];

    //extract instructions:
    std::vector<Instruction> instructions;
    for (int i = 2; i < v.size(); i++)
    {
        std::vector<String> slice = _chopString(v[i], MSG_CMD_SEP_C);
        if (slice.size() != 2)
        {
#ifdef DEBUG
            std::cout << "Error: invalid command in msg\n";
#endif
            return Message();
        }

        instructions.push_back(Instruction(slice[0], slice[1]));
    }
    if (instructions.size() < 1)
    {
#ifdef DEBUG
        std::cout << "Error: empty message\n";
#endif
        return Message();
    }

#ifdef DEBUG
    for (int i = 0; i < instructions.size(); i++)
        instructions[i].print();
    std::cout << "\n--end instructions--\n";
#endif

    //set the result message and return it
    result.instructions = instructions;
    result.msgTime = time(nullptr);
    return result;
}

String Message::messageToString(Message &message)
{
    if (message.isEmpty())
        return "";

    String result = "";
    result = MSG_START_C + std::to_string(message.length()) + MSG_SEP_C +
             message.id;

    for (int i = 0; i < message.instructions.size(); i++)
    {
        result += MSG_CMD_SEP_C + message.instructions[i].command +
                  MSG_CMD_SEP_C + message.instructions[i].value;
    }

    result += MSG_END_C;

    return result;
}

StationType Message::strToStationType(const String &str)
{
    if (str == NEW_INS_VALUE_PUSH)
        return StationType::push;
    else if (str == NEW_INS_VALUE_PULL)
        return StationType::pull;
    else if (str == NEW_INS_VALUE_BOTH)
        return StationType::both;
    else
        return StationType::undefined;
}

String Message::stationTypeToStr(const StationType &type)
{
    switch (type)
    {
    case StationType::push:
        return NEW_INS_VALUE_PUSH;
        break;
    case StationType::pull:
        return NEW_INS_VALUE_PULL;
        break;
    case StationType::both:
        return NEW_INS_VALUE_BOTH;
        break;
    default:
        return NEW_INS_VALUE_UNDEFINED;
        break;
    }
}

// void Message::print()
// {
//     if (this->isEmpty())
//         std::cout << std::endl;
//     else
//     {
//         std::cout << "{" << this->length() << MSG_SEP_C << this->id;
//         for (int i = 0; i < this->instructions.size(); i++)
//             this->instructions[i].print();

//         std::cout << "}" << std::endl;
//     }
// }

Instruction Message::getInstruction(String command)
{
    for (int i = 0; i < this->instructions.size(); i++)
        if (this->instructions[i].command == command)
            return this->instructions[i];

    return Instruction();
}

int Message::searchInstructionByIndex(String command)
{
    for (int i = 0; i < this->instructions.size(); i++)
        if (instructions[i].command == command)
            return i;

    return -1;
}

bool Message::hasInstruction(String command)
{
    return (this->searchInstructionByIndex(command) >= 0) ? true : false;
}

bool Message::_isNumber(const String &s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return false;
        }
    }

    return true;
}

int Message::length()
{
    if (this->isEmpty())
        return -1;

    int result = 3; //adds the {} and the 1st ;

    result += this->id.length();

    //for each command adds the ;C=V
    for (int i = 0; i < this->instructions.size(); i++)
        result += this->instructions[i].command.length() + this->instructions[i].value.length() + 2;

    //Adds the digits itself
    int lenDigitCount = static_cast<int>(log10(result)) + 1;
    result += static_cast<int>(log10(lenDigitCount + result)) + 1;

    return result;
}