// This code is used for sending probe requests every second.

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseArduino.h>
#include <time.h>


void setup() {
  Serial.begin(115200);
  // Set the sensor on ap and station mode
  WiFi.mode(WIFI_AP_STA);
}
int i = 0;
void loop() {
  // Print the mac address of the sensor
  Serial.print(WiFi.macAddress()+"\n");
  // When scanning for wifi AP probe requests are sent
  WiFi.scanNetworks();
  // Delay for one second
  delay(1000);
}
