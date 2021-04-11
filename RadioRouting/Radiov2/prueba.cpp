#include <ctime>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unistd.h>
#include <math.h>
#include <future>

#include "Message.h"

using namespace std;

enum StationType
{
    undefined,
    push,
    pull,
    both
};

class Stations
{
public:
    struct StationKey
    {
        int channel;
        std::string id;

        StationKey(int channel)
        {
            this->channel = channel;
            this->id = "";
        }

        StationKey(std::string id, int channel = -1)
        {
            this->channel = channel;
            this->id = id;
        }
    };

    struct StationValue
    {
        Message lastMessage;
        StationType stationType;

        StationValue()
        {
            this->stationType = undefined;
        }

        StationValue(StationType type)
        {
            this->stationType = type;
        }
    };

    struct orComparator
    {
        bool operator()(const StationKey &a, const StationKey &b) const
        {
            if (a.channel < b.channel)
            {
                return false;
            }
            else if (a.id < b.id)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    };

private:
public:
    std::map<StationKey, StationValue, orComparator> map;

    //Checks if the map has a station of given id
    bool hasStation(std::string id)
    {
        return map.find(StationKey(id)) != map.end();
    }

    // return =0 if succeed.
    bool addStation(std::string id, StationType type = StationType::undefined)
    {
        // If Id already exists, erase it first and replace
        if (this->hasStation(id))
        {
            this->map.erase(StationKey(id));
        }

        //Try 1 channel at a time until 1 channel is free and map accepts insertion
        bool insertionSucceeded = false;
        for (int ch = 1; !insertionSucceeded && ch < 50; ch++)
            insertionSucceeded = this->map.insert(std::pair<StationKey, StationValue>(
                                                      StationKey(id, ch), StationValue(type)))
                                     .second;

        if (insertionSucceeded)
            return true;
        else
            return false; //The router is full
    }

    void print()
    {
        for (auto it = map.begin(); it != map.end(); it++)
        {
            std::cout << "<ch=" << it->first.channel
                      << ", id=\'" << it->first.id
                      << "\'>:<type=" << it->second.stationType << ">" << std::endl;
        }
    }

    StationValue operator[](const int &value)
    {
        if (this->map.find(StationKey(value)) == this->map.end())
            return StationValue();
        else
            return this->map[StationKey(value)];
    }

    StationValue operator[](const std::string &id)
    {
        return this->map[StationKey(id)];
    }
};

int main()
{
    Stations stations;

    stations.addStation("/pozo", StationType::push);
    stations.addStation("/alberca", StationType::pull);
    stations.addStation("/molina", StationType::both);
    stations.addStation("/casaFer", StationType::both);

    stations.print();

    return 0;
}
