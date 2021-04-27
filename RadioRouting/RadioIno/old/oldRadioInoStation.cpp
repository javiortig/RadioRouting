// #include "RadioInoStation.h"

// /*
// RadioInoStation::RadioInoStation(const String &id, const StationType &type,
//                                  const int &rdPin, const int &tdPin,
//                                  const int &vccPin, const int &setPin): RadioDevice(rdPin, tdPin, vccPin, setPin)
// {
//     this->id = id;
//     this->type = type;

//     // Setup default messages
//     this->SYN_MSG.instructions.push_back(Instruction(SYN_CMD, TWH_DEFAULT_INS_VALUE));
//     this->SYN_MSG.id = this->id;

//     // Request the addition to the Router
//     this->requestAdd();
// }
// */
// bool RadioInoStation::TWH()
// {
//     //send SYN command
//     unsigned long start = millis();
//     unsigned long fin = millis();

//     Serial.println("Entering TWH");

//     do
//     {

//         this->sendMessage(this->SYN_MSG);
//         delay(SMALL_NAP_TIME * 2);

//         //check if synack received and id matches
//         this->buffer = this->readMessage();

//         if (!this->buffer.isEmpty() && this->buffer.hasInstruction(SYNACK_CMD))
//         {
//             //Check that id matches
//             if (this->buffer.id == MAIN_ROUTER_ID)
//             {
//                 Serial.print("TWH Successfull\n");
//                 return true;
//             }
//         }
//     } while (fin - start <= THW_WAIT_TIME * 1000 * 4);

//     Serial.print("TWH failed\n");
//     return false;
// }

// void RadioInoStation::requestAdd()
// {
//     Serial.print("Requesting add...\n");
//     Message addMessage(this->id, Instruction(NEW_CMD, this->type));
//     bool flag = false;

//     //Change to default channel. Blocking untill successfully changed channel
//     while(!this->setChannel(DEFAULT_CH));

//     // initialize digital pin LED_BUILTIN as an output.
//     pinMode(LED_BUILTIN, OUTPUT);

//     while (!flag)
//     {
//         digitalWrite(LED_BUILTIN, HIGH);
//         this->sendMessage(addMessage);

//         this->buffer = this->readMessage();

//         Serial.print("loop\n");

//         if (this->buffer.hasInstruction(SUCCESSFULLY_ADDED_CMD))
//         {
//             Serial.print("Hola\n");
//             int ch = this->buffer.getInstruction(SUCCESSFULLY_ADDED_CMD).value.toInt();
//             Serial.print("ch assigned: ");
//             Serial.println(String(ch));
//             if (ch <= 0)
//             {
//                 // Invalid conversion
//                 flag = false;
//                 continue;
//             }

//             //will be true only if successfully change channel
//             while (!this->setChannel(ch));
//             flag = true;
//         }

//         digitalWrite(LED_BUILTIN, LOW);
//         delay(SMALL_NAP_TIME* 1000);
//     }

//     Serial.print("Successfully added to the network by router\n");

//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(500);
//     digitalWrite(LED_BUILTIN, LOW);
//     delay(500);
//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(500);
//     digitalWrite(LED_BUILTIN, LOW);
//     delay(500);
//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(500);
//     digitalWrite(LED_BUILTIN, LOW);
// }
