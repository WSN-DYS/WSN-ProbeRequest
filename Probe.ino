#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <FirebaseESP8266.h>
#include <time.h>
#include "Hash.h" //for the encryption of the mac address


//creating the AP of the esp8266
const char* apSsid     = "Enter your created AP name";
const char* apPassword = "Enter your created AP password";

//connection details: WIFI name and password ** change for your wifi name and pass
const char* clientSsid = "Your AP name that you want to connect to";
const char* clientPassword = "Your AP password that you want to connect to";

// firebase authentication ** change for your firebase connection details
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Define blinking LED
#define LED 2

//the wifi handker for catching the probe requests
WiFiEventHandler probeRequestPrintHandler;

//convert from char* to string the mac address
String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}


std::vector<WiFiEventSoftAPModeProbeRequestReceived> myList; // save all the probe requests found to the vector
void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  myList.push_back(evt); // push each request to the vector
}

//Function that gets the channels priority and return them normilized for 1 minute routine.
//Input: vector of number of probes collected for each channel
//Return: the normalize vector
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

//printing the channels vector with the number of probe request for each channel
//Input: vector of number of probes collected for each channel
//Output: print the vector
void print_vector(std::vector<int> channels){
  for(int i = 0; i <13 ;  i++){
    Serial.print("number in channel ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(channels[i]);
  }
}

//Setup function for the ESP
void setup() {
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.println("Hello!");

  //Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);
  
  // start the esp on station ap mode
  WiFi.mode(WIFI_AP_STA);
  
  //Create access point 
  WiFi.softAP(apSsid, apPassword);
  
  //Conect to wifi AP
  WiFi.begin(clientSsid,clientPassword);

  //Check the connection to the internet
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  //Connect to firebase
  
  //Define the FirebaseConfig data for config data
  config.database_url = "proberequest-finalproject-default-rtdb.firebaseio.com";
  config.signer.tokens.legacy_token = "0b7kce2wu4NUsRNb1DEY3V7r8NNYYdVqxblZhXLd";
  
  //Assign the project host and api key 
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
  
  //Start the handler function for the collection of requests
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestPrint);
  
  //Time Set
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time update");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(5000);//5 seconds to restart time value 
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


  //String pathLocation = "/Users/"+WiFi.macAddress()+"/";//converting to const char for the push function path
  //FirebaseJson locationJson;//for the updated date
  //locationJson.add("Location",location);
  //Firebase.RTDB.updateNode(&fbdo, pathLocation.c_str() , &locationJson) ? "ok" : fbdo.errorReason().c_str();
  //delay(1000);
  

  while(counter <= channels[current_channel]){
    WiFi.softAP(apSsid, apPassword,current_channel +1);
    delay(1000);// delay for one second
  
      
   //get all probe requests and send to firebase
    for(WiFiEventSoftAPModeProbeRequestReceived w : myList){

    
      //creating FireBase json object
      FirebaseJson probe;
      FirebaseJson date;
      FirebaseJson RSSI;
      
      //creating time and substract the /n from it
      time_t now = time(nullptr);
      String timeNow = String(ctime(&now));
      timeNow.replace("\n","");

      //adding all the new variables to the json object
      probe.add("Sensor MAC",String(WiFi.macAddress()));
      probe.add("MAC",String(macToString(w.mac)));
      probe.add("MAC-SHA1",String(sha1(macToString(w.mac))));
      RSSI.add("RSSI",String(w.rssi));
      date.add("Time",timeNow);
      probe.add("Channel",String((current_channel +1)));
      
     
      //counting the number of each channel probes and the total vector for the statistics
      channels[current_channel]+=1;
      sum_of_all_probes_channels[current_channel] +=1;
      
      //push to firebase database
      String path = "/Users/"+WiFi.macAddress()+"/"+macToString(w.mac);//converting to const char for the push function path
      String timePath = "/Users/"+WiFi.macAddress()+"/"+macToString(w.mac)+"/Time";
      String RSSIPath = "/Users/"+WiFi.macAddress()+"/"+macToString(w.mac) + "/RSSI";
        

      
      //check if the mac address in already exists  
      if(Firebase.getString(fbdo,path + "/MAC")){
        
        FirebaseJson date2;//for the updated date
        FirebaseJson RSSI2;//for the updated rssi
        
        delay(1000);
        //update the date for existing address
        Firebase.RTDB.get(&fbdo, timePath.c_str());
        String tempDate = fbdo.jsonString();
        delay(1000);
        tempDate = tempDate.substring(9,tempDate.length()-2);
        timeNow = timeNow +", " + tempDate;
        date2.add("Time",timeNow);
        Firebase.RTDB.updateNode(&fbdo, timePath.c_str() , &date2 ) ? "ok" : fbdo.errorReason().c_str();  

        delay(1000);
        //update rssi for exist address
        Firebase.RTDB.get(&fbdo, RSSIPath.c_str());
        String newRSSI = fbdo.jsonString();
        delay(500);
        String tempRSSI = newRSSI.substring(9,newRSSI.length()-2);
        newRSSI = String(w.rssi) + ", " + tempRSSI;
        RSSI2.add("RSSI",newRSSI);
        Firebase.RTDB.updateNode(&fbdo, RSSIPath.c_str() , &RSSI2 ) ? "ok" : fbdo.errorReason().c_str();  
        delay(500);
        
      }else{
        //if the address not exist in the database push new json object
        Firebase.RTDB.set(&fbdo, path.c_str() , &probe) ? "ok" : fbdo.errorReason().c_str();
        Firebase.RTDB.set(&fbdo, timePath.c_str() , &date ) ? "ok" : fbdo.errorReason().c_str();
        Firebase.RTDB.set(&fbdo, RSSIPath.c_str() , &RSSI ) ? "ok" : fbdo.errorReason().c_str();
    
      
      }
    }
    
    
    //clear the list from all old mac addresses found
    myList.clear();
    digitalWrite(LED, LOW);
    
    delay(1000);//delay for 1 seconds
    counter+=2;//add 2 seconds to timer
  }
  //counter to zero to count number of probes in net channel and move the current channel one up
  counter = 0;
  current_channel +=1;

  // using the normalize algo 
  if(current_channel ==13){
    current_channel =0;
    normilized_channels(channels);
    Serial.println("Toal vector of probes: ");
    print_vector(sum_of_all_probes_channels);
  }
  
}
