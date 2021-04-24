#include "Message.h"

void setup()
{
    Serial.begin(115200);
    delay(500);
    unsigned char k = 95;
    k = k / 10;
    Serial.println(k);

    // Message buffer("{22;pozo;val=true;2=a}");
    // bool a = buffer.insertInstruction("pepe", "está");
    // bool b = buffer.insertInstruction("pepeasdfsad", "está");
    // buffer.print();
}

void loop()
{
    /*if(station.newInstructions.size() < 6)
     station.addInstructionToSend(Instruction("Lleno","True"));
      
  station.routine();
  Serial.print("loop\n");
  delay(5000);*/
}