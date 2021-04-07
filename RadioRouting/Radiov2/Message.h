#pragma once

#include <iostream>
#include <vector>

class Message
{
    struct Instruction
    {
        std::string command;
        std::string value;

        Instruction()
        {
            this->command = nullptr;
            this->value = nullptr;
        }

        template <class C, class V>
        Instruction(C command, V value)
        {
            this->command = command;
            this->value = value;
        }
    };

public:
    int len;
    std::vector<Instruction> instructions;
    time_t time;

    Message strToMessage(char *str);

    Message();
    Message(std::vector<Instruction> instructions);
};