// #pragma once

// #include "RadioInoStation.h"

// class RadioInoStationPush : public RadioInoStation
// {
// private:

//     // This vector will delete the instruction each msgTime the instruction is sent
//     public: std::vector<Instruction> newInstructions;

// public:
//     // Returns true if successfully contacted with the Router
//     void routine();

//     //called by user before routine to send a message
//     void addInstructionToSend(const Instruction &instruction);

//     void start(const String &id,
//                     const int &rdPin = HC12_RD, const int &tdPin = HC12_TD,
//                     const int &vccPin = HC12_VCC, const int &setPin = HC12_SET);

//     RadioInoStationPush(){};
// };
