#include "Message.h"

SoftwareSerial s(1, 2);

Message buffer;

void setup()
{
    Serial.begin(115200);
    delay(500);

    Serial.println(sizeof(buffer));
}

void loop()
{
    /*if(station.newInstructions.size() < 6)
     station.addInstructionToSend(Instruction("Lleno","True"));
      
  station.routine();
  Serial.print("loop\n");
  delay(5000);*/
}