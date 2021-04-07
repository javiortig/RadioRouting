#include <ctime>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    time_t startT, endT;

    startT = time(nullptr);
    int b;
    sleep(2.456f);

    endT = time(nullptr);
    cout << "hola:\n";
    cout << endT - startT << endl;

    return 0;
}