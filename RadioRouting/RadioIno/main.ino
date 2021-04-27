#include "Message.h"
#define ID "PILo"

Message m;

void setup()
{
    Serial.begin(115200);
    delay(500);
    m.createDefault(MAIN_ROUTER_ID);
    m.print();

    if (m.compareId("pozo"))
        Serial.println(F("SI"));
    else
        Serial.println(F("NO"));
}

void loop()
{
}