#include "Message.h"

void Message::indexOf(unsigned char *res, const char str[], const char &value, unsigned char from)
{
    for (*res = from + 2; str[*res] != '\0'; ++(*res))
    {
        if (str[*res] == value)
        {
            return;
        }
    }

    *res = CHAR_ERROR;
}

bool Message::strToMessage(const char str[])
{

    unsigned char sPos, ePos, sAux, eAux;
    indexOf(&sPos, str, MSG_START_C);
    indexOf(&ePos, str, MSG_END_C);

    // Invalid message. save empty
    if (sPos < 0 || ePos < 0 || strlen(str) < 3 || sPos + 1 >= strlen(str) || !isdigit(str[sPos + 1]))
    {
        this->makeEmpty();
        return false;
    }
    //Check the messageLen

    if (v.size() <= 2)
    {
        return oldMessage(); // oldMessage is Empty
    }

    //extract number and check if it matches len
    int msgLen = v[0].toInt();

    if (msgLen != str.length() + 2)
    {
        return oldMessage();
    }

    //extract id:
    result.id = v[1];

    Serial.println("LLego1");

    //extract instructions:
    std::vector<Instruction> instructions;
    std::vector<String> slice;

    Serial.println("Sizes:");
    Serial.println(sizeof(str));
    Serial.println(sizeof(result));
    Serial.println(sizeof(instructions));
    Serial.println(sizeof(slice));
    Serial.println(sizeof(v));

    for (int i = 2; i < v.size(); i++)
    {
        Serial.println("Iter " + String(i));
        slice = _chopString(v[i], MSG_CMD_SEP_C);
        if (slice.size() != 2)
        {
            return oldMessage();
        }

        instructions.push_back(Instruction(slice[0], slice[1]));
        slice.clear();
    }

    if (instructions.size() < 1)
    {
        return oldMessage();
    }

    //set the result message and return it
    result.instructions = instructions;

    Serial.println("salgo strtomsg");
    result.print();
    return result;
}

void Message::makeEmpty()
{
    this->value[0].makeEmpty();
}

bool Message::isEmpty()
{
    return this->value[0].isEmpty();
}