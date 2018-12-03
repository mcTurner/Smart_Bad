/* 
"SmartBad_ESP8266_V0.01"

Pinbelegung:

Relai       Pin 0
Tempsensor  Pin 2

Funktion: veröffentlicht Temperatur &Luftfeuchtigkeit über mqtt, schaltet heizung AN/AUS
kommende funktion: emfängt soll temperatur in °C

*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <SimpleDHT.h>
#include "T_Objects.h"

///////////////////////////////////////////////////////////////////////////Settings/////////////////////////////////////////////
const int Heizkontakt_HP = 0;
const int Relai_2 = 25;
const int pinDHT11 = 02;
const char* Code_Version = "SmartBad_ESP8266_V0.01";

// Update these with values suitable for your network.
const char* ssid = "Turner.Netz";
const char* password = "3333333333333";
const char* mqttBrokerIP = "openhabianpi";
const int  mqttBrokerPORT = 1883;
const char* OTAHostname = "Bad_ESP_8266_1";

long BadheizerHandle_delay = 10000;
long readtemp_delay = 60000;

////////////////////////////////////////////////////////////////////////////// Variabeln//////////////////////////////////////////

// Topics
const char* subTopic1 = "Haus/EG/Bad/Heizer";
const char* subTopic2 = "Haus/EG/Bad/TempSoll";
const char* subTopic3 = "";

const char* pubTopic0 = "Haus/EG/Bad/Sensor/Temp";
const char* pubTopic1 = "Haus/EG/Bad/Sensor/Hum";
const char* pubTopic2 = "";
const char* pubTopic3 = "";

//Timing
long lastMsg = 0;
int value = 0;
long last_readtemp_time = 0;
long last_BadheizerHandle_time = 0;

// message
char msg[50];
//Tempsensor Var
byte  Temp = 0;
byte  Hum = 0;
byte temperature = 0;
byte humidity = 0;

///////////////////////////////////////////////////////////////////////////Objekte/Klassen///////////////////////////////////////////////

SimpleDHT11 dht11(pinDHT11);
WiFiClient wifiClient;
PubSubClient client(wifiClient);
Heizobjekt Badheizer(Heizkontakt_HP,20);
///////////////////////////////////////////////////////////////////////////Funktionen/////////////////////////////////////////////////////
void readtemp() {
  Serial.println("Sample DHT11...");
  dht11.read(&temperature, &humidity, NULL);

}

///////////////////////////////////////////////////////////////////////////MQTT Callback///////////////
void callback(char* topic, byte* payload, unsigned int length) 
{

  String callTopic = "";
  int i = 0;
  while((char)topic[i] != 0) {
    callTopic += (char)topic[i];
    i++;
  } //parses topic from bytes

  if (callTopic.equals(subTopic1)) {
    Serial.println("Topic match subTopic1");
    Serial.println((char)payload[0]);
    if((char)payload[0] == '1') {
      
      Badheizer.power_off();
      Serial.println("Badheizer.power_off()");  
  
    } else if ((char)payload[0] == '0'){
      Badheizer.power_on();
      Serial.println("Badheizer.power_on()");  
    }    
  }

  if (callTopic.equals(subTopic2)) {
    Serial.println("Topic match subTopic2");
    if((char)payload[0] == '0') {
      //
    } else if ((char)payload[0] == '1'){
      //
    }    
  }

  if (callTopic.equals(subTopic3)) {
    if((char)payload[0] == '0') {
      //outletOff(2);
    } else if ((char)payload[0] == '1'){
      //outletOn(2);
    }    
  }
}


/**
 * This function is called when the loop methond sees there is no
 * MQTT connection.  This function attempts to reconnect.  If it can 
 * not, it disconnects from wifi, connects again, and attempts to connect to 
 * the MQTT server again, once wifi is established.
 */
void reconnect() 
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("YourDeviceName")) {
      Serial.println("connected");

      client.setServer(mqttBrokerIP, mqttBrokerPORT);
      client.setCallback(callback);
  
      client.subscribe(subTopic1);
      client.subscribe(subTopic2);
      //client.subscribe(subTopic3);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(", resetting wif connection.");
      
      delay(500);
      WiFi.disconnect();
      Serial.println("Disconnected from WiFi");
      delay(1000);
      connectWifi();
      Serial.println("Wifi Reconnected!  Trying MQTT Connection again.");
      delay(5000);
    }
  }
}

///////////////////////////////////////////////////////////////////////////Wifi Setup/////////////////////////////////////////////////////
void connectWifi() 
{
  delay(200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(150);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi: Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void arduinoOTA(){

 // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  Serial.println("OTA Ready");
  
}

/////////////////////////////////////////////////////////////////// Setup    ///////////////////////////////////////////
/*
 * First function to run.
 */
void setup() 
{
  Serial.begin(115200);
 Serial.println(Code_Version);
 Serial.println("Booting");
 
  pinMode(Heizkontakt_HP, OUTPUT);
  pinMode(Relai_2, OUTPUT);
  digitalWrite(Heizkontakt_HP, HIGH); 
  digitalWrite(Relai_2, HIGH);
  connectWifi();
  arduinoOTA();
  delay(1000);
  client.setServer(mqttBrokerIP, mqttBrokerPORT);
  client.setCallback(callback);
  
  if(client.connected()) {
    Serial.println("MQTT: Connected");
  }
  
}



/////////////////////////////////////////////////////////////////// Loop    ///////////////////////////////////////////

/**
 * This function is called repeatedly.  It calls the client's loop function (required)
 * and handles the connection state if something happens in the network.
 */
void loop() 
{
  if(!client.connected()) {
    reconnect();
  }
  ArduinoOTA.handle();
  client.loop();
  long now;
  /*
  now = millis();
  
  if (now - last_readtemp_time > readtemp_delay){
    //Serial.println(now - last_readtemp_time);
    last_readtemp_time = now;
    readtemp();
    s0nprintf (msg, 75, "%d", temperature);
    client.publish("WZ/Sensor/Temp", msg);
    snprintf (msg, 75, "%d", humidity);
    client.publish("WZ/Sensor/Hum", msg);
    }*/
   
   now = millis();
   if (now - last_BadheizerHandle_time > BadheizerHandle_delay){
   last_BadheizerHandle_time = now;
   Badheizer.handle();
   } 
}
