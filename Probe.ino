#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseArduino.h>
#include <time.h>


const char* apSsid     = "MyMesh";
const char* apPassword = "123456";
const char* clientSsid     = "Enter local network SSID";
const char* clientPassword = "enter local network PASS";

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
  
  //TIME SET
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  
}
int channel = 1;
void loop() {

  //create the Json object fot the users information
  String json = "";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& probes = root.createNestedArray("probes");

  //channel changeing
  WiFi.softAP(apSsid, apPassword,(channel%13) +1);
  channel +=1;

 //get all probe requests and send to firebase
  for(WiFiEventSoftAPModeProbeRequestReceived w : myList){
    
    //create Json structure to the user information and enter thr information
    JsonObject& probe = probes.createNestedObject();
    time_t now = time(nullptr);
    probe["Sensor MAC"] = WiFi.macAddress();
    probe["MAC"] = macToString(w.mac);
    probe["rssi"] = w.rssi;
    probe["Time"] = ctime(&now);
    
    

    //push to firebase database
    Firebase.set("Users/"+WiFi.macAddress()+"/"+macToString(w.mac),probe);
    
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(30000);//delay for 30 seconds 
  }
  Serial.println("json:" + json);

  //pushing to the database the mac asdresses and the rssi
  
  myList.clear();
  root.printTo(json);
  
}
