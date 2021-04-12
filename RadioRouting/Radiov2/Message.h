#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <chrono>

#define MSG_START_C '{'
#define MSG_END_C '}'
#define MSG_SEP_C ';'
#define MSG_CMD_SEP_C '='

#define SYN_CMD "SYN"
#define SYNACK_CMD "SAK"
#define FINISH_CMD "FIN"
#define OK_CMD "OK"

//The new command requesting being added to the network.
//The command values correspond to the StationType
#define NEW_CMD "NEW"
#define NEW_INS_VALUE_PUSH "PUSH"
#define NEW_INS_VALUE_PULL "PULL"
#define NEW_INS_VALUE_BOTH "BOTH"

#define EMPTY_INSTRUCTION ""

#define TWH_DEFAULT_INS_VALUE 0

struct Instruction
{
    std::string command;
    std::string value;

    Instruction()
    {
        this->command = EMPTY_INSTRUCTION;
        this->value = EMPTY_INSTRUCTION;
    }

    template <class C, class V>
    Instruction(C command, V value)
    {
        this->command = command;
        this->value = value;
    }

    bool isEmpty()
    {
        return (this->command == EMPTY_INSTRUCTION) ? true : false;
    }

    void print()
    {
        std::cout << this->command << MSG_CMD_SEP_C << this->value << MSG_SEP_C;
    }
};

class Message
{

private:
    static int _reverseNumber(int num);
    static std::vector<std::string> _chopString(std::string str, char sep);
    static bool _isNumber(const std::string &s);

public:
    //Attributes:
    std::string id;
    std::vector<Instruction> instructions;
    time_t time;

    //Methods:
    bool isEmpty();
    static Message strToMessage(std::string str);
    static std::string messageToString(Message &message);
    void print();
    Instruction getInstruction(std::string command);
    //Returns index of instruction in the msg or -1
    int searchInstructionByIndex(std::string command);
    //Same as search, but only returns true or false if match
    bool hasInstruction(std::string command);

    int length();

    Message(std::string id, std::vector<Instruction> instructions);
    Message(std::string messageStr);
    Message(){};
};