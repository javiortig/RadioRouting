#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <chrono>

#define MSG_START_C '{'
#define MSG_END_C '}'
#define MSG_SEP_C ';'
#define MSG_CMD_SEP_C '='

#define SYN_CMD "SYN"
#define SYNACK_CMD "SAK"
#define FINISH_CMD "FIN"
#define OK_CMD "OK"
// The value of this instruction is the channel
#define SUCCESSFULLY_ADDED_CMD "SUCC"

//The new command requesting being added to the network.
//The command values correspond to the StationType
#define NEW_CMD "NEW"
#define NEW_INS_VALUE_PUSH "PUSH"
#define NEW_INS_VALUE_PULL "PULL"
#define NEW_INS_VALUE_BOTH "BOTH"
#define NEW_INS_VALUE_UNDEFINED "UNDF"

#define EMPTY_INSTRUCTION ""

#define TWH_DEFAULT_INS_VALUE 0

/*
    {len;id;CMD=VALUE;CMD=VALUE}
*/

enum StationType
{
    undefined,
    push,
    pull,
    both
};

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

    void print(bool sep = true)
    {
        if (sep)
            std::cout << MSG_SEP_C;

        std::cout << this->command << MSG_CMD_SEP_C << this->value;
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
    time_t msgTime;

    //Methods:
    bool isEmpty();

    static Message strToMessage(std::string str);
    static std::string messageToString(Message &message);

    static std::string stationTypeToStr(const StationType &type);
    static StationType strToStationType(const std::string &str);

    void print();
    Instruction getInstruction(std::string command);
    //Returns index of instruction in the msg or -1
    int searchInstructionByIndex(std::string command);
    //Same as search, but only returns true or false if match
    bool hasInstruction(std::string command);

    int length();

    Message(const std::string &id, const std::vector<Instruction> &instructions);
    Message(const std::string &id, const Instruction &singleInstruction);
    Message(const std::string &messageStr);
    Message(){};
};