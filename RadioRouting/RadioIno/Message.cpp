#include "Message.h"

Message::Message()
{
    this->id[0] = '\0';
}

Message::Message(const char *str)
{
    this->strToMessage(str);
}

void Message::indexOf(unsigned char *res, const char str[], const char &value)
{
    for (; str[*res] != '\0'; ++(*res))
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

    unsigned char sPos = 0, ePos = 0, sAux = 0, eAux = 0, iAux = 0;
    unsigned char x, y;
    indexOf(&sPos, str, MSG_START_C);
    indexOf(&ePos, str, MSG_END_C);

    // Invalid message. save empty
    if (sPos < 0 || ePos < 0 || strlen(str) < 3 || sPos + 1 >= strlen(str) || !isdigit(str[sPos + 1]))
    {
        this->makeEmpty();
        return false;
    }
    //Check the messageLen
    sAux = sPos;
    eAux = sAux;
    ++eAux;
    indexOf(&eAux, str, MSG_SEP_C);

    Serial.println(sAux);

    for (y = 0, x = 0, iAux = eAux - 1; iAux > sAux; ++y, --iAux)
    {
        Serial.println(String(iAux) + "-" + String(str[iAux]));
        if (!isDigit(str[iAux]))
        {
            this->makeEmpty();
            return false;
        }
        x += pow(10, y) * (str[iAux] - '0');
    }

    Serial.println(String(strlen(str)) + "!=" + String(x));
    //check if matches
    if (strlen(str) != x)
    {
        this->makeEmpty();
        return false;
    }

    //extract id:
    Serial.println(F("Llego extraact id"));
    for (sAux = ++eAux, x = 0; str[sAux] != MSG_SEP_C && str[sAux] != '\0'; ++sAux, ++x)
    {
        if (x >= MAX_ID_SIZE)
        {
            this->makeEmpty();
            return false;
        }
        else
        {
            this->id[x] = str[sAux];
        }
    }
    if (str[sAux] != MSG_SEP_C)
    {
        this->makeEmpty();
        return false;
    }
    else
    {
        this->id[x] = '\0';
    }

    Serial.println(F("Llego instructions"));

    /*
        extract instructions:
        -x is the index for num of instructions
        -y used as a flag to know wether we are on command or value in each Instruction
         0 for command, 1 for value
        -iAux to index through the instruction
        -eAux to index the main str
    */
    for (eAux = ++sAux, x = 0, y = 0, iAux = 0; str[eAux] != MSG_END_C && str[eAux] != '\0'; ++eAux, ++iAux)
    {
        Serial.println("str[eAux]=" + String(str[eAux]) + " /x=" + x + " /y=" + y + " iAux=" + iAux);
        if (x >= MESSAGE_MAX_INSTRUCTIONS || iAux >= INSTRUCTION_SIZE)
        {
            this->makeEmpty();
            return false;
        }

        //change from command to value
        if (str[eAux] == MSG_INS_SEP_C)
        {
            if (y != 0)
            {
                this->makeEmpty();
                return false;
            }
            y = 1;
            this->value[x].command[iAux] = '\0';
            iAux = -1;
            continue;
        }
        // change to next instruction
        else if (str[eAux] == MSG_SEP_C)
        {
            if (y != 1)
            {
                this->makeEmpty();
                return false;
            }
            y = 0;
            this->value[x].value[iAux] = '\0';
            iAux = -1;
            ++x;
            continue;
        }
        //assign value
        else
        {
            switch (y)
            {
            case 0:
                this->value[x].command[iAux] = str[eAux];
                break;

            case 1:
                this->value[x].value[iAux] = str[eAux];
                break;

            default:
                this->makeEmpty();
                return false;
                break;
            }
        }
    }
    if (str[eAux] != MSG_END_C)
    {
        this->makeEmpty();
        return false;
    }
    this->value[x].value[iAux] = '\0';

    return true;
}

void Message::makeEmpty()
{
    this->value[0].makeEmpty();
}

bool Message::isEmpty()
{
    return this->value[0].isEmpty();
}

unsigned char Message::len()
{
    if (this->isEmpty())
    {
        return 0;
    }
    unsigned char result = 3, i; //adds the {} and the 1st ;

    result += strlen(this->id);

    //for each command adds the ;C=V
    for (i = 0; i < MESSAGE_MAX_INSTRUCTIONS && !this->value[i].isEmpty(); ++i)
        result += strlen(this->value[i].command) + strlen(this->value[i].value) + 2;

    //Adds the digits itself
    i = static_cast<unsigned char>(log10(result)) + 1;
    result += static_cast<unsigned char>(log10(i + result)) + 1;

    return result;
}

bool Message::insertInstruction(const char *c, const char *v)
{
    if (strlen(c) > INSTRUCTION_SIZE || strlen(v) > INSTRUCTION_SIZE)
        return false;

    unsigned char i;
    for (i = 0; i < MESSAGE_MAX_INSTRUCTIONS; ++i)
    {
        if (this->value[i].isEmpty())
        {
            strcpy(this->value[i].command, c);
            strcpy(this->value[i].value, v);
            return true;
        }
    }

    return false;
}

void Message::print()
{
    if (this->isEmpty())
    {
        Serial.println(F("Empty"));
    }
    else
    {
        Serial.print(MSG_START_C);
        Serial.print(static_cast<int>(this->len()));
        Serial.print(MSG_SEP_C);
        Serial.print(this->id);
        for (unsigned char i = 0; i < MESSAGE_MAX_INSTRUCTIONS && !this->value[i].isEmpty(); ++i)
            this->value[i].print();

        Serial.println(MSG_END_C);
    }
}