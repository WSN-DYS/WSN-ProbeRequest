#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseArduino.h>

const char* apSsid     = "MyMesh";
const char* apPassword = "123456";
const char* clientSsid     = "----------";
const char* clientPassword = "-----------";

// firebase authentication
#define FIREBASE_HOST "proberequest-finalproject-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "0b7kce2wu4NUsRNb1DEY3V7r8NNYYdVqxblZhXLd"

WiFiEventHandler probeRequestPrintHandler;

String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

std::vector<WiFiEventSoftAPModeProbeRequestReceived> myList;

void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  myList.push_back(evt);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSsid, apPassword);
  WiFi.begin(clientSsid, clientPassword);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to the firebase database
  
  Serial.println("");
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestPrint);

  
}

void loop() {
  delay(3000);
  String json = "";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& probes = root.createNestedArray("probes");
  for(WiFiEventSoftAPModeProbeRequestReceived w : myList){
    JsonObject& probe = probes.createNestedObject();
    probe["address"] = macToString(w.mac);
    probe["rssi"] = w.rssi;
    //Firebase.setString("Users/"+macToString(w.mac),"RSSI/"+w.rssi);
    Firebase.set("Users/"+macToString(w.mac),probe);
    //Firebase.setString("Users:/" +macToString(w.mac),"RSSI"+w.rssi);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(100);
  }
  Serial.println("json:" + json);

  //pushing to the database the mac asdresses and the rssi
  
  myList.clear();
  root.printTo(json);
  
}
