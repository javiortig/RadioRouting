#include "Message.h"

Message m;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  m = Message("{30;PP;/pozo=False;/door=open}");

  m.print();
  
  delay(2000);
}
