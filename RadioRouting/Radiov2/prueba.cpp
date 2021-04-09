#include <ctime>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unistd.h>

#include "Message.h"

// Example: {len;id;command...}

int main()
{
    auto start = std::chrono::system_clock::now();

    sleep(4.12);
    int j = 0;
    for (size_t i = 0; i < 10000; i++)
    {
        j = i + 1;
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << elapsed_seconds.count() << std::endl;
    return 0;
}
