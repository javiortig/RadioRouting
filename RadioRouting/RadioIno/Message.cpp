#include "Message.h"

bool Message::validateMessage()
{
    unsigned char x, y;
    this->sIndex = this->value;
    this->eIndex = ++this->sIndex;

    if (this->value[0] != MSG_START_C)
        return false;
    //check that msgLen matches
    for (x = 1; *eIndex != MSG_SEP_C; ++x, ++eIndex)
    {
        if (x >= MSG_SIZE || *eIndex == '\0' || !isdigit(*eIndex))
        {
            return false;
        }
    }
    for (y = 0, x = 0, sIndex = eIndex - 1; *sIndex != MSG_START_C; ++y, --sIndex)
    {

        x += pow(10, y) * (*sIndex - '0');
    }
    if (strlen(this->value) != x)
        return false;

    //Check that has id
    ++eIndex;
    if (*eIndex == MSG_SEP_C || *eIndex == MSG_END_C || *eIndex == '\0')
        return false;

    //check instructions
    //Move to first instruction
    for (; *eIndex != MSG_SEP_C; ++eIndex)
    {
        if (*eIndex == '\0')
            return false;
    }
    //Check that each instruction has the pattern ;c=v
    for (x = 0, ++eIndex; *eIndex != MSG_END_C && *eIndex != '\0'; ++eIndex)
    {
        if (*eIndex == MSG_SEP_C)
        {
            if (x == 1)
            {
                x = 0;
                continue;
            }
            else
            {
                return false;
            }
        }

        if (*eIndex == MSG_INS_SEP_C)
        {
            if (x == 0)
            {
                x = 1;
                continue;
            }
            else
            {
                return false;
            }
        }
    }

    if (*eIndex != MSG_END_C || x != 1)
        return false;

    return true;
}

bool Message::findInstruction(const char *command)
{
    unsigned char x;
    //skip len and id
    for (x = 0, sIndex = value; x < 2; ++sIndex)
    {
        if (*sIndex == MSG_SEP_C)
        {
            ++x;
        }
    }

    for (eIndex = sIndex, x = 0; *eIndex != '\0'; ++eIndex)
    {
        if (command[x] == '\0' && *eIndex == MSG_INS_SEP_C)
        {
            sIndex = eIndex + 1;
            for (++eIndex; *eIndex != MSG_END_C && *eIndex != MSG_SEP_C; ++eIndex)
                ;
            return true;
        }

        if (*eIndex == command[x])
        {
            ++x;
        }
        else
        {
            x = 0;
        }
    }

    return false;
}

void Message::createDefault(const char *id)
{
    /*
        {999;id}
        sIndex to index value
        eIndex to index id
    */
    sIndex = value;
    *sIndex = MSG_START_C;
    for (++sIndex; sIndex - value < MSG_LEN_LEN + 1; ++sIndex)
    {
        *sIndex = '1';
    }
    *sIndex = MSG_SEP_C;
    for (++sIndex, eIndex = (char *)id; *eIndex != '\0'; ++sIndex, ++eIndex)
    {
        *sIndex = *eIndex;
    }

    *sIndex = MSG_END_C;
    ++sIndex;
    *sIndex = '\0';
}

bool Message::insertInstruction(const char *c, const char *v)
{
    unsigned char i, j;

    //i to make sure we dont pass the MSG_SIZE.
    //sIndex to track where MSG_END_C was in chase we need to redo
    //j to loop c and v
    for (i = 0; value[i] != MSG_END_C; ++i)
        ;
    value[i] = MSG_SEP_C;

    //copy command.
    for (sIndex = value + i++, j = 0; c[j] != '\0'; ++i, ++j)
    {
        if (i >= MSG_SIZE)
        {
            *sIndex = MSG_END_C;
            *(sIndex + 1) = '\0';
            return false;
        }
        else
        {
            value[i] = c[j];
        }
    }

    //copy the = character
    if (i >= MSG_SIZE)
    {
        *sIndex = MSG_END_C;
        *(sIndex + 1) = '\0';
        return false;
    }
    value[i] = MSG_INS_SEP_C;

    //copy value.
    for (++i, j = 0; v[j] != '\0'; ++i, ++j)
    {
        if (i >= MSG_SIZE)
        {
            *sIndex = MSG_END_C;
            *(sIndex + 1) = '\0';
            return false;
        }
        else
        {
            value[i] = v[j];
        }
    }
    if (i >= MSG_SIZE + 1)
    {
        *sIndex = MSG_END_C;
        *(sIndex + 1) = '\0';
        return false;
    }

    value[i] = MSG_END_C;
    value[++i] = '\0';

    /*
        i for strlen
        eIndex to index the value arr
    */
    i = strlen(this->value);
    for (eIndex = this->value + MSG_LEN_LEN; eIndex > this->value; --eIndex, i /= 10)
    {
        *eIndex = (i % 10) + '0';
    }

    return true;
}

void Message::print()
{
    Serial.println(this->value);
}

bool Message::isEmpty()
{
    return this->value[0] == '\0';
}

bool Message::compareId(const char *str)
{
    if (this->isEmpty())
        return false;

    eIndex = value;
    while (*eIndex != MSG_SEP_C)
        ++eIndex;

    sIndex = ++eIndex;
    while (*eIndex != MSG_SEP_C && *eIndex != MSG_END_C)
        ++eIndex;

    return (!strncmp(sIndex, str, strlen(MAIN_ROUTER_ID)));
}

// Message::Message()
// {
//     this->id[0] = '\0';
// }

// Message::Message(const char *str)
// {
//     this->strToMessage(str);
// }

// void Message::indexOf(unsigned char *res, const char str[], const char &value)
// {
//     for (; str[*res] != '\0'; ++(*res))
//     {
//         if (str[*res] == value)
//         {
//             return;
//         }
//     }

//     *res = CHAR_ERROR;
// }

// bool Message::strToMessage(const char str[])
// {

//     unsigned char sPos = 0, ePos = 0, sAux = 0, eAux = 0, iAux = 0;
//     unsigned char x, y;
//     indexOf(&sPos, str, MSG_START_C);
//     indexOf(&ePos, str, MSG_END_C);

//     // Invalid message. save empty
//     if (sPos < 0 || ePos < 0 || strlen(str) < 3 || sPos + 1 >= strlen(str) || !isdigit(str[sPos + 1]))
//     {
//         this->makeEmpty();
//         return false;
//     }
//     //Check the messageLen
//     sAux = sPos;
//     eAux = sAux;
//     ++eAux;
//     indexOf(&eAux, str, MSG_SEP_C);

//     Serial.println(sAux);

//     for (y = 0, x = 0, iAux = eAux - 1; iAux > sAux; ++y, --iAux)
//     {
//         Serial.println(String(iAux) + "-" + String(str[iAux]));
//         if (!isDigit(str[iAux]))
//         {
//             this->makeEmpty();
//             return false;
//         }
//         x += pow(10, y) * (str[iAux] - '0');
//     }

//     Serial.println(String(strlen(str)) + "!=" + String(x));
//     //check if matches
//     if (strlen(str) != x)
//     {
//         this->makeEmpty();
//         return false;
//     }

//     //extract id:
//     Serial.println(F("Llego extraact id"));
//     for (sAux = ++eAux, x = 0; str[sAux] != MSG_SEP_C && str[sAux] != '\0'; ++sAux, ++x)
//     {
//         if (x >= MAX_ID_SIZE)
//         {
//             this->makeEmpty();
//             return false;
//         }
//         else
//         {
//             this->id[x] = str[sAux];
//         }
//     }
//     if (str[sAux] != MSG_SEP_C)
//     {
//         this->makeEmpty();
//         return false;
//     }
//     else
//     {
//         this->id[x] = '\0';
//     }

//     Serial.println(F("Llego instructions"));

//     /*
//         extract instructions:
//         -x is the index for num of instructions
//         -y used as a flag to know wether we are on command or value in each Instruction
//          0 for command, 1 for value
//         -iAux to index through the instruction
//         -eAux to index the main str
//     */
//     for (eAux = ++sAux, x = 0, y = 0, iAux = 0; str[eAux] != MSG_END_C && str[eAux] != '\0'; ++eAux, ++iAux)
//     {
//         Serial.println("str[eAux]=" + String(str[eAux]) + " /x=" + x + " /y=" + y + " iAux=" + iAux);
//         if (x >= MESSAGE_MAX_INSTRUCTIONS || iAux >= INSTRUCTION_SIZE)
//         {
//             this->makeEmpty();
//             return false;
//         }

//         //change from command to value
//         if (str[eAux] == MSG_INS_SEP_C)
//         {
//             if (y != 0)
//             {
//                 this->makeEmpty();
//                 return false;
//             }
//             y = 1;
//             this->value[x].command[iAux] = '\0';
//             iAux = -1;
//             continue;
//         }
//         // change to next instruction
//         else if (str[eAux] == MSG_SEP_C)
//         {
//             if (y != 1)
//             {
//                 this->makeEmpty();
//                 return false;
//             }
//             y = 0;
//             this->value[x].value[iAux] = '\0';
//             iAux = -1;
//             ++x;
//             continue;
//         }
//         //assign value
//         else
//         {
//             switch (y)
//             {
//             case 0:
//                 this->value[x].command[iAux] = str[eAux];
//                 break;

//             case 1:
//                 this->value[x].value[iAux] = str[eAux];
//                 break;

//             default:
//                 this->makeEmpty();
//                 return false;
//                 break;
//             }
//         }
//     }
//     if (str[eAux] != MSG_END_C)
//     {
//         this->makeEmpty();
//         return false;
//     }
//     this->value[x].value[iAux] = '\0';

//     return true;
// }

// void Message::makeEmpty()
// {
//     this->value[0].makeEmpty();
// }

// bool Message::isEmpty()
// {
//     return this->value[0].isEmpty();
// }

// unsigned char Message::len()
// {
//     if (this->isEmpty())
//     {
//         return 0;
//     }
//     unsigned char result = 3, i; //adds the {} and the 1st ;

//     result += strlen(this->id);

//     //for each command adds the ;C=V
//     for (i = 0; i < MESSAGE_MAX_INSTRUCTIONS && !this->value[i].isEmpty(); ++i)
//         result += strlen(this->value[i].command) + strlen(this->value[i].value) + 2;

//     //Adds the digits itself
//     i = static_cast<unsigned char>(log10(result)) + 1;
//     result += static_cast<unsigned char>(log10(i + result)) + 1;

//     return result;
// }

// bool Message::insertInstruction(const char *c, const char *v)
// {
//     if (strlen(c) > INSTRUCTION_SIZE || strlen(v) > INSTRUCTION_SIZE)
//         return false;

//     unsigned char i;
//     for (i = 0; i < MESSAGE_MAX_INSTRUCTIONS; ++i)
//     {
//         if (this->value[i].isEmpty())
//         {
//             strcpy(this->value[i].command, c);
//             strcpy(this->value[i].value, v);
//             return true;
//         }
//     }

//     return false;
// }

// void Message::print()
// {
//     if (this->isEmpty())
//     {
//         Serial.println(F("Empty"));
//     }
//     else
//     {
//         Serial.print(MSG_START_C);
//         Serial.print(static_cast<int>(this->len()));
//         Serial.print(MSG_SEP_C);
//         Serial.print(this->id);
//         for (unsigned char i = 0; i < MESSAGE_MAX_INSTRUCTIONS && !this->value[i].isEmpty(); ++i)
//             this->value[i].print();

//         Serial.println(MSG_END_C);
//     }
// }