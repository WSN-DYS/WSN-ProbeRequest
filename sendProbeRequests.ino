#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseArduino.h>
#include <time.h>


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP_STA);
}
int i = 0;
void loop() {
  Serial.print(WiFi.macAddress()+"\n");
  WiFi.scanNetworks();
  delay(1000);
}
