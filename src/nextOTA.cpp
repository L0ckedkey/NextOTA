#include "nextOTA.h"

NextOTA::NextOTA() {}

void NextOTA::begin(const String& ip) {
  serverIP = ip;
  Serial.println("Initiate communication to server with IP : " + serverIP);
}

void NextOTA::connect(const String& name) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String url = "http://" + serverIP + ":3000/register";
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    String ip = WiFi.localIP().toString();
    String mac = WiFi.macAddress();
    boardType = name;
    String postData = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"mac\":\"" + WiFi.macAddress() + "\",\"name\":\"" + boardType + "\"}";

    int httpResponseCode = http.POST(postData);

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void NextOTA::keepAlive() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        WiFiClient client;

        String url = "http://" + serverIP + ":3000/alive";
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");

        String mac = WiFi.macAddress();
        String postData = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"mac\":\"" + WiFi.macAddress() + "\",\"name\":\"" + boardType + "\"}";

        int httpResponseCode = http.POST(postData);

        http.end();
    }
}
