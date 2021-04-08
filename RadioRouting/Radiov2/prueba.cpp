#include <ctime>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#include "Message.h"

// Example: {len;command...}

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

std::vector<std::string> chopString(std::string str, char sep)
{
    //Chop message
    std::vector<std::string> result;
    std::stringstream ss(str);

    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, sep);
        result.push_back(substr);
    }

    return result;
}

int main()
{
    std::string str = "_{7;a=1}";

    Message m = Message::strToMessage(str);
    std::cout << "Lenn is: " << m.len << std::endl;
    m.print();
    return 0;
}
