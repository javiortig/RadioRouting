#include "RadioPiRouter.h"

using namespace std;
/*
    {len;id;CMD=VALUE;CMD=VALUE}
*/

void handleStationInstructions(std::string id, std::vector<Instruction> instructions)
{
    cout << "Instructions received:\n";
    cout << "id: " << id << "\nInstructions: ";
    for (auto it = instructions.begin(); it != instructions.end(); it++)
    {
        it->print();
    }
}

std::vector<Instruction> getInstructionsForStation(std::string id)
{
    cout << "Instruction requested for id: " << id << "\n";
}

int main()
{
    if (gpioInitialise() < 0)
    {
        // pigpio initialisation failed.
        return -1;
    }

    RadioPiRouter router();
    cout << "Creado\n";

    while (true)
    {
        router.routine(&handleStationInstructions, &getInstructionsForStation);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    gpioTerminate();
    return 0;
}
