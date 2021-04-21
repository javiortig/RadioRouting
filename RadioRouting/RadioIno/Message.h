#pragma once

#include <Arduino.h>
#include <time.h>
#include <SoftwareSerial.h>

#define MSG_START_C '{'
#define MSG_END_C '}'
#define MSG_SEP_C ';'
#define MSG_INS_SEP_C '='

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

#define STATIONTYPE_UNDEF 'U'
#define STATIONTYPE_PUSH 'S'
#define STATIONTYPE_PULL 'L'
#define STATIONTYPE_BOTH 'B'

#define INSTRUCTION_SIZE 5 // num of chars in command and value
#define MESSAGE_SIZE 4     //  num of instructions per message
#define MAX_ID_SIZE 5

#define CHAR_ERROR 255

struct Instruction
{
    char command[INSTRUCTION_SIZE];
    char value[INSTRUCTION_SIZE];

    bool isEmpty()
    {
        return (this->command[0] == '\0' && this->value[0] == '\0') ? true : false;
    }

    void makeEmpty()
    {
        this->command[0] = '\0';
        this->value[0] = '\0';
    }

    void print()
    {
        unsigned char i;

        if (this->isEmpty())
            return;

        Serial.print(MSG_SEP_C);
        Serial.print(this->command);
        Serial.print(MSG_INS_SEP_C);
        Serial.print(this->value);
    }

    Instruction()
    {
        this->makeEmpty();
    }
};

class Message
{
public:
    char id[MAX_ID_SIZE];
    Instruction value[MESSAGE_SIZE];

    void indexOf(unsigned char *res, const char str[], const char &value, unsigned char from = 0);

public:
    //Modifies this object from a received string
    bool strToMessage(const char str[]);
    void messageToString(char *str_out);

    //Makes this object empty
    void makeEmpty();
    bool isEmpty();
};