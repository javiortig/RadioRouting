#include "RadioInoStationPush.h"

RadioInoStationPush station;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  station.start("pozo");
}

void loop()
{
  if (station.newInstructions.size() < 6)
    station.addInstructionToSend(Instruction("Lleno", "True"));

  station.routine();
  Serial.print("loop\n");
  delay(5000);
}
