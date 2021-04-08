#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>

#define MSG_START_C '{'
#define MSG_END_C '}'
#define MSG_SEP_C ';'
#define MSG_CMD_C '='

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

        void print()
        {
            std::cout << this->command << MSG_CMD_C << this->value << MSG_SEP_C;
        }
    };

private:
    static int _reverseNumber(int num);
    static std::vector<std::string> _chopString(std::string str, char sep);

public:
    int len;
    std::vector<Instruction> instructions;
    time_t time;

    static Message strToMessage(std::string str);

    void print();

    Message();
    Message(std::vector<Instruction> instructions);
};