#include "nextOTA.h"

NextOTA::NextOTA() {}

void NextOTA::begin(const String& ip) {
  serverIP = ip;
  Serial.println("Initiate communication to server with IP : " + serverIP);
}

void NextOTA::connect(const String& boardType) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String url = "http://" + serverIP + ":3000/register";
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    String ip = WiFi.localIP().toString();
    String mac = WiFi.macAddress();

    DynamicJsonDocument jsonDoc(1024);  // Adjust the size if needed
    jsonDoc["ip"] = WiFi.localIP().toString();
    jsonDoc["mac"] = WiFi.macAddress();
    jsonDoc["boardType"] = boardType;

    String postData;
    serializeJson(jsonDoc, postData);

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}