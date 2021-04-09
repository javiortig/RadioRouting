#include "Message.h"

Message::Message(std::vector<Instruction> instructions)
{
    this->instructions = instructions;
}

Message::Message(std::string messageStr)
{
    *this = Message::strToMessage(messageStr);
}

std::vector<std::string> Message::_chopString(std::string str, char sep)
{
    //Chop message
    std::vector<std::string> result;
    std::stringstream ss(str);

    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, sep);
        result.push_back(substr);
    }

    return result;
}

bool Message::isEmpty()
{
    return this->instructions.empty();
}

Message Message::strToMessage(std::string str)
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
    std::vector<std::string> v = _chopString(str, MSG_SEP_C);

#ifdef DEBUG
    std::cout << "Full msg string: " << str << " with len: " << str.length() << std::endl;
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
        std::cout << "Error: msg len doesnt match\n";
#endif
        return Message();
    }
    //extract id:
    if (_isNumber(v[1]))
    {
        result.id = std::stoi(v[1]);
    }
    else
    {
#ifdef DEBUG
        std::cout << "Error: no id provided in msg\n";
#endif
        return Message();
    }

    //extract instructions:
    std::vector<Instruction> instructions;
    for (int i = 2; i < v.size(); i++)
    {
        std::vector<std::string> slice = _chopString(v[i], MSG_SEP_C);
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
    std::cout << "--end instructions--\n";
#endif

    //set the result message and return it
    result.instructions = instructions;
    result.time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return result;
}

std::string Message::messageToString(Message &message)
{
    if (message.isEmpty())
        return "";

    std::string result = "";
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

void Message::print()
{
    if (this->isEmpty())
        std::cout << std::endl;
    else
    {
        for (int i = 0; i < this->instructions.size(); i++)
            this->instructions[i].print();

        std::cout << std::endl;
    }
}

Instruction Message::getInstruction(std::string command)
{
    for (int i = 0; i < this->instructions.size(); i++)
        if (this->instructions[i].command == command)
            return this->instructions[i];

    return Instruction();
}

int Message::searchInstructionByIndex(std::string command)
{
    for (int i = 0; i < this->instructions.size(); i++)
        if (instructions[i].command == command)
            return i;

    return -1;
}

bool Message::hasInstruction(std::string command)
{
    return (this->searchInstructionByIndex(command) >= 0) ? true : false;
}

bool Message::_isNumber(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
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