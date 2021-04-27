// #include "RadioInoStationPush.h"

// void RadioInoStationPush::start(const String &id,
//                     const int &rdPin, const int &tdPin,
//                     const int &vccPin, const int &setPin)
// {
//     this->hc12RDPin = rdPin;
//     this->hc12TDPin = tdPin;
//     this->hc12VCCPin = vccPin;
//     this->hc12SetPin = setPin;
//     this->id = id;
//     this->type = StationType::push;

//     this->startHC12();

//     // Setup default messages
//     this->SYN_MSG.instructions.push_back(Instruction(SYN_CMD, TWH_DEFAULT_INS_VALUE));
//     this->SYN_MSG.id = this->id;

//     // Request the addition to the Router
//     this->requestAdd();
// }

// void RadioInoStationPush::routine()
// {
//     //If we have instructions to send, send all to router
//     if (!this->newInstructions.empty())
//     {
//         if (!this->TWH())
//         {
//             return;
//         }

//         Serial.println("TWH successfull. Sending Instructions");

//         Message data(this->id, this->newInstructions);

//         //Clear the instructions:
//         this->newInstructions.clear();

//         this->sendMessage(data);
//         delay(SMALL_NAP_TIME * 1000);
//         this->sendMessage(data);
//         delay(SMALL_NAP_TIME * 1000);
//         this->sendMessage(data);
//     }

//     Serial.println("Ending routine...");
// }

// void RadioInoStationPush::addInstructionToSend(const Instruction &instruction)
// {
//     this->newInstructions.push_back(instruction);
// }
