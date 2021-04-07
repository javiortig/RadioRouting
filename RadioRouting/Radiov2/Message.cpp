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

Message Message::strToMessage(char *str)
{
}