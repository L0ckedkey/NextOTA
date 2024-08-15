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

        if (httpResponseCode > 0) {
            Serial.println("Register response code: " + String(httpResponseCode));
        } else {
            Serial.println("Error in registration: " + String(httpResponseCode));
        }

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

        if (httpResponseCode > 0) {
            Serial.println("Keep alive response code: " + String(httpResponseCode));
        } else {
            Serial.println("Error in keep alive: " + String(httpResponseCode));
        }

        http.end();
    }
}

void NextOTA::updateFirmware() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        WiFiClient client;
        
        // Modify URL to point to the server's endpoint for firmware distribution
        String url = "http://" + serverIP + ":3000/update";
        http.begin(client, url);
        
        int httpResponseCode = http.GET();

        if (httpResponseCode == HTTP_CODE_OK) {
            int contentLength = http.getSize();
            if (contentLength > 0) {
                bool canBegin = Update.begin(contentLength);
                if (canBegin) {
                    WiFiClient* stream = http.getStreamPtr();
                    size_t written = Update.writeStream(*stream);
                    
                    if (written == contentLength) {
                        if (Update.end()) {
                            if (Update.isFinished()) {
                                Serial.println("Update completed");
                                // Optionally, you may want to reset the board here
                                // ESP.restart();
                            } else {
                                Serial.println("Update not finished");
                            }
                        } else {
                            Serial.println("Update failed");
                        }
                    } else {
                        Serial.println("Write failed");
                    }
                } else {
                    Serial.println("Not enough space to begin OTA");
                }
            }
        } else {
            Serial.println("Firmware download failed, HTTP response code: " + String(httpResponseCode));
        }
        http.end();
    } else {
        Serial.println("Error in WiFi connection");
    }
}
