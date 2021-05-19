#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseArduino.h>
#include <time.h>
#include "Hash.h" //for the encryption of the mac address

//creating the AP of the esp8266
const char* apSsid     = "MyMesh";
const char* apPassword = "123456";

//The info for connecting as a client to WiFi access point
const char* clientSsid     = "Dasa";
const char* clientPassword = "2222266666";

// firebase authentication
#define FIREBASE_HOST "proberequest-finalproject-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "0b7kce2wu4NUsRNb1DEY3V7r8NNYYdVqxblZhXLd"

//Define blinking LED
#define LED 2

WiFiEventHandler probeRequestPrintHandler;

//convert from char* to string the mac address
String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}


std::vector<WiFiEventSoftAPModeProbeRequestReceived> myList; // save all the probe requests found
void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  myList.push_back(evt);
}

//function that gets the channels priority and return them normilized for 1 minute.
void normilized_channels(std::vector<int> &channels){
  float sum_of_all = 0;
  for(auto i = channels.begin(); i!= channels.end() ; i++){
    sum_of_all += *i ;
  }
  float normalize_sum = sum_of_all / 60;
  for(auto i = 0; i < 13 ; i++){
    channels[i] = channels[i] / normalize_sum ;
  }
  Serial.println("normalize vector:");
  print_vector(channels); 
}

void print_vector(std::vector<int> channels){
  for(int i = 0; i <13 ;  i++){
    Serial.print("number in channel ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(channels[i]);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.println("Hello!");

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

 
  WiFi.mode(WIFI_AP_STA);// start the esp on station ap mode
  
  // create access point 
  WiFi.softAP(apSsid, apPassword);
  
  // conect to wifi AP
  WiFi.begin(clientSsid,clientPassword);

  //check the connection to the internet
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  //connect to firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to the firebase database
  
  Serial.println("");
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestPrint);
  
  //TIME SET
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time to restart");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  
}

int channel = 0;
std::vector<int> channels (13,4); //vector to count the probe channels
std::vector<int> sum_of_all_probes_channels (13,0); //vector to count the probe channels
int counter = 0;
int current_channel = 0;
  

void loop() {
  digitalWrite(LED, HIGH);
  //create the Json object fot the users information
  String json = "";

  //all the information saved in a json structure fot the firebase database
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& probes = root.createNestedArray("probes");
  
  //print_vector(channels);
  //channel changeing for getting all the probes in all channels
  while(counter <= channels[current_channel]){
    WiFi.softAP(apSsid, apPassword,current_channel +1);
    //channel +=1;
    //Serial.println(channels[current_channel]);
    delay(1000);// delay for one second
  
     
      
   //get all probe requests and send to firebase
    for(WiFiEventSoftAPModeProbeRequestReceived w : myList){
      
      //create Json structure to the user information and enter thr information
      JsonObject& probe = probes.createNestedObject();
      time_t now = time(nullptr);
      //all the information from the probe requetsts gathered here before pushed to the firebase
      probe["Sensor MAC"] = WiFi.macAddress(); // the sendor MAC
      probe["MAC"] = macToString(w.mac);//The client Mac
      probe["MAC-SHA1"] = sha1(macToString(w.mac));//Client mac hash
      probe["rssi"] = w.rssi;//The signal strength from the esp
      probe["Time"] = ctime(&now);// the time the probe catched
      probe["Channel"] = (current_channel +1);//which channel the probe request was sent from
      
      //counting the number of each channel probes and the total vector for the statistics
      channels[current_channel]+=1;
      sum_of_all_probes_channels[current_channel] +=1;
      
      //push to firebase database
      Firebase.set("Users/"+WiFi.macAddress()+"/"+macToString(w.mac),probe);
      
      // handle error
      if (Firebase.failed()) {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());  
        return;
      }
      
      //print on the serial for testing the esp
      Serial.println(macToString(w.mac));
      Serial.println(sha1(macToString(w.mac)));
      Serial.println(current_channel +1);
    }
    
    
    //pushing to the database the mac asdresses and the rssi
    myList.clear();
    root.printTo(json);
    digitalWrite(LED, LOW);
    
    delay(1000);//delay for 1 seconds
    counter+=2;
  }
  counter = 0;
  current_channel +=1;

  if(current_channel ==13){
    current_channel =0;
    normilized_channels(channels);
    Serial.println("Toal vector of probes: ");
    print_vector(sum_of_all_probes_channels);
  }
}
