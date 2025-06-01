#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "********";
const char* password = "********";

// Google Apps Script Web App URL
const char* serverName = "Put ur URL here";
// Equipment Name
const char* equipmentName = "Generator GEN-0420";

// Send interval (1/12 minute)
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1 * 5 * 1000UL;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
}

void loop() {
  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();
    sendDataToGoogleSheets();
  }
}

// Send simulated data to Google Sheets
void sendDataToGoogleSheets() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Skipping data send.");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");

  // Simulate values around target readings
  float workHours   = 110.0 + random(-50, 50) / 10.0;
  float temperature = 60.5 + random(-30, 30) / 10.0;
  float oilPressure = 9.3 + random(-20, 20) / 10.0;

  String jsonData = "{\"equipment\":\"" + String(equipmentName) +
                    "\",\"workHours\":\"" + String(workHours, 2) +
                    "\",\"temperature\":\"" + String(temperature, 1) +
                    "\",\"oilPressure\":\"" + String(oilPressure, 1) + "\"}";

  Serial.println("Sending JSON: " + jsonData);
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.println("Response: " + http.getString());
  } else {
    Serial.print("POST failed: ");
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end();
}
