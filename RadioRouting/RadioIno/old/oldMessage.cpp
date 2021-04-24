// #include "oldMessage.h"

// oldMessage::oldMessage(const String &id, const std::vector<Instruction> &instructions)
// {
//     this->instructions = instructions;
//     this->id = id;
// }

// oldMessage::oldMessage(const String &id, const Instruction &singleInstruction)
// {
//     this->instructions.push_back(singleInstruction);
//     this->id = id;
// }

// oldMessage::oldMessage(const String &messageStr)
// {
//     *this = oldMessage::strToMessage(messageStr);
// }

// std::vector<String> oldMessage::_chopString(const String &str, const char &delim)
// {
//     std::vector<String> cont;
//     int startInd = 0, nextDelInd = 0;
//     nextDelInd = str.indexOf(delim, startInd);
//     while (nextDelInd > -1)
//     {
//         cont.push_back(str.substring(startInd, nextDelInd));
//         startInd = nextDelInd + 1;
//         nextDelInd = str.indexOf(delim, startInd);
//     }

//     cont.push_back(str.substring(startInd));

//     return cont;
// }

// bool oldMessage::isEmpty()
// {
//     return this->instructions.empty();
// }

// oldMessage oldMessage::strToMessage(String str)
// {
//     oldMessage result;

//     str.trim();

//     int sPos = str.indexOf(MSG_START_C);
//     int ePos = str.indexOf(MSG_END_C);
//     // Invalid message. return empty
//     if (sPos < 0 || ePos < 0 || str.length() < 3 || sPos + 1 >= str.length() || !isdigit(str[sPos + 1]))
//     {
//         return oldMessage();
//     }
//     str = str.substring(1, str.length() - 1);
//     std::vector<String> v = _chopString(str, MSG_SEP_C);

//     if (v.size() <= 2)
//     {
//         return oldMessage(); // oldMessage is Empty
//     }

//     //extract number and check if it matches len
//     int msgLen = v[0].toInt();

//     if (msgLen != str.length() + 2)
//     {
//         return oldMessage();
//     }

//     //extract id:
//     result.id = v[1];

//     Serial.println("LLego1");

//     //extract instructions:
//     std::vector<Instruction> instructions;
//     std::vector<String> slice;

//     Serial.println("Sizes:");
//     Serial.println(sizeof(str));
//     Serial.println(sizeof(result));
//     Serial.println(sizeof(instructions));
//     Serial.println(sizeof(slice));
//     Serial.println(sizeof(v));

//     for (int i = 2; i < v.size(); i++)
//     {
//         Serial.println("Iter " + String(i));
//         slice = _chopString(v[i], MSG_CMD_SEP_C);
//         if (slice.size() != 2)
//         {
//             return oldMessage();
//         }

//         instructions.push_back(Instruction(slice[0], slice[1]));
//         slice.clear();
//     }

//     if (instructions.size() < 1)
//     {
//         return oldMessage();
//     }

//     //set the result message and return it
//     result.instructions = instructions;

//     Serial.println("salgo strtomsg");
//     result.print();
//     return result;
// }

// String oldMessage::messageToString(oldMessage &message)
// {
//     if (message.isEmpty())
//         return "";

//     String result = "";
//     result = MSG_START_C + String(message.length()) + MSG_SEP_C +
//              message.id;

//     for (int i = 0; i < message.instructions.size(); i++)
//     {
//         result += MSG_SEP_C + message.instructions[i].command +
//                   MSG_CMD_SEP_C + message.instructions[i].value;
//     }

//     result += MSG_END_C;

//     return result;
// }

// StationType oldMessage::strToStationType(const String &str)
// {
//     if (str == NEW_INS_VALUE_PUSH)
//         return StationType::push;
//     else if (str == NEW_INS_VALUE_PULL)
//         return StationType::pull;
//     else if (str == NEW_INS_VALUE_BOTH)
//         return StationType::both;
//     else
//         return StationType::undefined;
// }

// String oldMessage::stationTypeToStr(const StationType &type)
// {
//     switch (type)
//     {
//     case StationType::push:
//         return NEW_INS_VALUE_PUSH;
//         break;
//     case StationType::pull:
//         return NEW_INS_VALUE_PULL;
//         break;
//     case StationType::both:
//         return NEW_INS_VALUE_BOTH;
//         break;
//     default:
//         return NEW_INS_VALUE_UNDEFINED;
//         break;
//     }
// }

// void oldMessage::print()
// {
//     if (this->isEmpty())
//         Serial.print("Empty\n");
//     else
//     {
//         Serial.print("{" + String(this->length()) + MSG_SEP_C + this->id);
//         for (int i = 0; i < this->instructions.size(); i++)
//             this->instructions[i].print();

//         Serial.print("}\n");
//     }
// }

// Instruction oldMessage::getInstruction(String command)
// {
//     for (int i = 0; i < this->instructions.size(); i++)
//         if (this->instructions[i].command == command)
//             return this->instructions[i];

//     return Instruction();
// }

// int oldMessage::searchInstructionByIndex(String command)
// {
//     for (int i = 0; i < this->instructions.size(); i++)
//         if (instructions[i].command == command)
//             return i;

//     return -1;
// }

// bool oldMessage::hasInstruction(String command)
// {
//     return (this->searchInstructionByIndex(command) >= 0) ? true : false;
// }

// bool oldMessage::_isNumber(const String &s)
// {
//     for (int i = 0; i < s.length(); i++)
//     {
//         if (s[i] < '0' || s[i] > '9')
//         {
//             return false;
//         }
//     }

//     return true;
// }

// int oldMessage::length()
// {
//     if (this->isEmpty())
//         return -1;

//     int result = 3; //adds the {} and the 1st ;

//     result += this->id.length();

//     //for each command adds the ;C=V
//     for (int i = 0; i < this->instructions.size(); i++)
//         result += this->instructions[i].command.length() + this->instructions[i].value.length() + 2;

//     //Adds the digits itself
//     int lenDigitCount = static_cast<int>(log10(result)) + 1;
//     result += static_cast<int>(log10(lenDigitCount + result)) + 1;

//     return result;
// }