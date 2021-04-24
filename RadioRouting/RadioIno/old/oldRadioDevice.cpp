// #include "oldRadioDevice.h"
// /*
// RadioDevice::RadioDevice(const int &rdPin, const int &tdPin,
//                                  const int &vccPin, const int &setPin)
// {
//     this->hc12RDPin = rdPin;
//     this->hc12TDPin = tdPin;
//     this->hc12VCCPin = vccPin;
//     this->hc12SetPin = setPin;

//     this->startHC12();
// }*/

// /*
// RadioDevice::~RadioDevice(){
//   if (this->hc12 != nullptr)
//     delete this->hc12;
// }
// */
// void RadioDevice::sendMessage(String messageStr)
// {
//     this->sendMessage(Message(messageStr));
// }

// bool RadioDevice::setChannel(int channel)
// {
//     Serial.print("voy a cambiar de canal a:");
//     Serial.println(String(channel));

//     String smsg = "AT+C0";
//     String temp = "";
//     String confirm = "OK+C0";

//     if ((channel < MIN_CH || channel > MAX_CH) && channel != DEFAULT_CH)
//         return false;
//     if (channel < 10)
//     {
//         smsg += '0';
//         confirm += '0';
//     }
//     smsg += String(channel);
//     confirm += String(channel);

//     digitalWrite(this->hc12SetPin, LOW);
//     delay(300);

//     this->writeHC12(smsg);
//     temp = this->readHC12();

//     if (temp != confirm)
//     {
//         digitalWrite(this->hc12SetPin, HIGH);

//         Serial.print("Error in setChannel\n");

//         delay(100);
//         return false;
//     }

//     digitalWrite(this->hc12SetPin, HIGH);

//     Serial.print("Channel changed successfully\n");

//     delay(120);
//     this->currentChannel = channel;
//     return true;
// }

// void RadioDevice::startHC12()
// {
//     pinMode(this->hc12VCCPin, OUTPUT);
//     digitalWrite(this->hc12VCCPin, HIGH);
//     pinMode(this->hc12SetPin, OUTPUT);
//     digitalWrite(this->hc12SetPin, HIGH);
//     delay(200);
//     this->hc12 = new SoftwareSerial(this->hc12RDPin, this->hc12TDPin);
//     delay(200);
//     this->hc12->begin(9600);
// }

// void RadioDevice::stopHC12()
// {
//     this->hc12->end();
//     delay(200);

//     digitalWrite(this->hc12VCCPin, LOW);
// }

// void RadioDevice::writeHC12(const String &str)
// {
//     //clear hc12
//     while (hc12->available())
//     {
//         hc12->read();
//     }

//     hc12->print(str);
//     delay(HC12_DELAY_M);
// }

// String RadioDevice::readHC12()
// {
//     delay(HC12_DELAY_M);
//     String b = "";
//     while (hc12->available())
//     {
//         b += char(hc12->read());
//     }
//     b.trim();

//     if (b.length() > 0)
//     {
//         Serial.print("readHC12(): ");
//         Serial.println(b);
//     }

//     return b;
// }

// Message RadioDevice::readMessage()
// {
//     return Message::strToMessage(this->readHC12());
// }

// void RadioDevice::sendMessage(Message message)
// {
//     while (hc12->available())
//     {
//         hc12->read();
//     }

//     hc12->print(Message::messageToString(message));
// }
