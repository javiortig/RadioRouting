#include <ctime>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unistd.h>
#include <math.h>

#include "Message.h"

// Example: {len;id;command...}

int main()
{
    int inLen = 10;
    int v = static_cast<int>(log10(inLen)) + 1;
    v = static_cast<int>(log10(inLen + v)) + 1;

    std::cout << v << std::endl;
    return 0;
}
