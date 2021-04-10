#include <ctime>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unistd.h>
#include <math.h>

#include "Message.h"

// Example: {len;id;command...}

void blockingWait(float seconds)
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    while (elapsed_seconds.count() <= seconds)
    {
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
    }
}

int main()
{
    float seconds = 1;

    std::cout << "Waiting for: " << seconds << std::endl;
    blockingWait(seconds);
    std::cout << "Finished" << std::endl;
    return 0;
}
