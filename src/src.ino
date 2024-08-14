#include "nextOTA.h"
#include <ESP8266WiFi.h>

const char* ssid = "A 1105B";
const char* password = "12345678";

NextOTA nextOTA;

void setup() 
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  nextOTA.begin("192.168.100.97");
  nextOTA.connect("asd");
}

void loop() {}