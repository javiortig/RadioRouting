#pragma once

#include <Arduino.h>
#include <time.h>
#include <SoftwareSerial.h>
#include <math.h>

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

#define EMPTY_INSTRUCTION_VALUE "E"

/*
    len has always 3 digits
    {len;id;CMD=VALUE;CMD=VALUE}
*/

#define MAIN_ROUTER_ID "RTR"

#define CHAR_ERROR 255

#define MSG_SIZE 40
#define MSG_LEN_LEN 3

class Message
{
public:
    char value[MSG_SIZE];
    char *sIndex;
    char *eIndex;

    //checks if the current message contained is valid
    bool validateMessage();

    //returns true if successfully found. If found, it will move sIndex to the
    //First character of the instructions value and eIndex to the end non-inclusive
    bool findInstruction(const char *command);

    void makeEmpty();
    bool isEmpty();

    //writes in value the default message of the given id with no instruction.
    // An Instruction must be inserted before trying sending this message
    void createDefault(const char *id);
    bool insertInstruction(const char *c, const char *v);

    void print();

    //Compares the actual id of the message with the given id on str
    bool compareId(const char *str);
};

// struct Instruction
// {
//     char command[INSTRUCTION_SIZE];
//     char value[INSTRUCTION_SIZE];

//     bool isEmpty()
//     {
//         return (this->command[0] == '\0' && this->value[0] == '\0') ? true : false;
//     }

//     void makeEmpty()
//     {
//         this->command[0] = '\0';
//         this->value[0] = '\0';
//     }

//     void print()
//     {
//         unsigned char i;

//         if (this->isEmpty())
//             return;

//         Serial.print(MSG_SEP_C);
//         Serial.print(this->command);
//         Serial.print(MSG_INS_SEP_C);
//         Serial.print(this->value);
//     }

//     Instruction()
//     {
//         this->makeEmpty();
//     }
// };

// class Message
// {
// public:
//         char id[MAX_ID_SIZE];
//         Instruction value[MESSAGE_MAX_INSTRUCTIONS];

//         void indexOf(unsigned char *res, const char str[], const char &value);

//     public:
//         Modifies this object from a received string
//         bool strToMessage(const char str[]);

//         Writes the actual message in the given string str_out. Returns empty of failed
//         void messageToString(char *str_out);

//         Makes this object empty
//         void makeEmpty();
//         bool isEmpty();

//         bool insertInstruction(const char *c, const char *v);

//         void print();

//         unsigned char len();

//         Message();
//         Message(const char *str);
// };