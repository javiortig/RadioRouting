#include "Message.h"

Message::Message()
{
    this->instructions = std::vector<Instruction>();
    this->len = -1;
    this->time = -1;
}

Message::Message(std::vector<Instruction> instructions)
{
    this->instructions = instructions;
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

    if (v.size() <= 1)
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
    //extract instructions:
    std::vector<Instruction> instructions;
    for (int i = 1; i < v.size(); i++)
    {
        std::vector<std::string> slice = _chopString(v[i], MSG_CMD_C);
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
    result.len = msgLen;

    return result;
}

void Message::print()
{
    if (this->len < 1)
        std::cout << std::endl;
    else
    {
        for (int i = 0; i < this->instructions.size(); i++)
            this->instructions[i].print();

        std::cout << std::endl;
    }
}