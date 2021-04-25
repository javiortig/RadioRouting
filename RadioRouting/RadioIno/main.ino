#include "Message.h"

Message m;

void setup()
{
    Serial.begin(115200);
    delay(500);
    strcpy(m.value, "{31;pozo;value=true;val=si;v=s}");
    m.print();
    Serial.println(m.validateMessage());
}

void loop()
{
}