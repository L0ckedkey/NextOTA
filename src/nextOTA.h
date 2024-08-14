#ifndef NEXTOTA_H
#define NEXTOTA_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h> 

class NextOTA {
  public:
    NextOTA();
    void begin(const String& ip);
    void connect(const String& boardType);
  
  private:
    String serverIP;
};

#endif
