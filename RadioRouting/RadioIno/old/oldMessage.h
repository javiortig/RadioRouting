// #pragma once

// #include <Arduino.h>
// #include <time.h>
// #include <SoftwareSerial.h>

// #include <MemoryUsage.h>

// #include <ArduinoSTL.h>

// #define MSG_START_C '{'
// #define MSG_END_C '}'
// #define MSG_SEP_C ';'
// #define MSG_CMD_SEP_C '='

// #define SYN_CMD "SYN"
// #define SYNACK_CMD "SAK"
// #define FINISH_CMD "FIN"
// #define OK_CMD "OK"
// // The value of this instruction is the channel
// #define SUCCESSFULLY_ADDED_CMD "SUCC"

// //The new command requesting being added to the network.
// //The command values correspond to the StationType
// #define NEW_CMD "NEW"
// #define NEW_INS_VALUE_PUSH "PUSH"
// #define NEW_INS_VALUE_PULL "PULL"
// #define NEW_INS_VALUE_BOTH "BOTH"
// #define NEW_INS_VALUE_UNDEFINED "UNDF"

// #define EMPTY_INSTRUCTION ""

// #define TWH_DEFAULT_INS_VALUE 0

// /*
//     {len;id;CMD=VALUE;CMD=VALUE}
// */

// enum StationType
// {
//     undefined,
//     push,
//     pull,
//     both
// };

// struct Instruction
// {
//     String command;
//     String value;

//     Instruction()
//     {
//         this->command = EMPTY_INSTRUCTION;
//         this->value = EMPTY_INSTRUCTION;
//     }

//     template <class C, class V>
//     Instruction(C command, V value)
//     {
//         this->command = command;
//         this->value = value;
//     }

//     bool isEmpty()
//     {
//         return (this->command == EMPTY_INSTRUCTION) ? true : false;
//     }

//     void print(bool sep = true)
//     {
//         if (sep)
//             Serial.print(MSG_SEP_C);

//         Serial.print(this->command + MSG_CMD_SEP_C + this->value);
//     }
// };

// class oldMessage
// {

// private:
//     static int _reverseNumber(int num);
//     static std::vector<String> _chopString(const String &str, const char &sep);
//     static bool _isNumber(const String &s);

// public:
//     //Attributes:
//     String id;
//     std::vector<Instruction> instructions;
//     time_t msgTime;

//     //Methods:
//     bool isEmpty();

//     static oldMessage strToMessage(String str);
//     static String messageToString(oldMessage &message);

//     static String stationTypeToStr(const StationType &type);
//     static StationType strToStationType(const String &str);

//     void print();
//     Instruction getInstruction(String command);
//     //Returns index of instruction in the msg or -1
//     int searchInstructionByIndex(String command);
//     //Same as search, but only returns true or false if match
//     bool hasInstruction(String command);

//     int length();

//     oldMessage(const String &id, const std::vector<Instruction> &instructions);
//     oldMessage(const String &id, const Instruction &singleInstruction);
//     oldMessage(const String &messageStr);
//     oldMessage(){};
// };