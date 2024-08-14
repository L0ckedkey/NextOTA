#include "nextOTA.h"

NextOTA nextOTA;

void setup() 
{
  nextOTA.begin("10.22.65.109");
  nextOTA.connect("asd");
  Serial.begin(9600);
}

void loop() {}