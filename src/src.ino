#include "nextOTA.h"
#include <ESP8266WiFi.h>

const char* ssid = "NA Office";
const char* password = "NaCakepCakep";

NextOTA nextOTA;

void setup() 
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  nextOTA.begin("10.22.65.109");
  nextOTA.connect("asd");
  nextOTA.updateFirmware();
}

void loop() {
  nextOTA.keepAlive();
}