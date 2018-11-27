/* 
Smart_Bad Projekt Version 0.01

Pinbelegung:

Relai       Pin
Tempsensor  Pin


*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <SimpleDHT.h>

const int Relay_1 = 17;

////////////////////////////////////////////////////////////////////////////// Variabeln//////////////////////////////////////////
// Update these with values suitable for your network.
const char* ssid = "Turner.Netz";
const char* password = "3333333333333";
const char* mqttBrokerIP = "192.168.1.115";
const int  mqttBrokerPORT = 1883;
// Topics
const char* subTopic = "Haus/EG/WZ/Licht";
const char* subTopic2 = "Haus/EG/WZ/Licht1";
const char* subTopic3 = "Haus/EG/WZ/Licht3";

const char* pubTopic = "Haus/EG/WZ/Sensor/Temp";
const char* pubTopic2 = "Haus/EG/WZ/Licht1";
const char* pubTopic3 = "Haus/EG/WZ/Licht3";

//Timing
long lastMsg = 0;

int value = 0;
long last_readtemp_time = 0;
// message
char msg[50];
//Tempsensor Var
long readtemp_delay = 60000;
long last_readtemp_time = 0;
byte  Temp = 0;
byte  Hum = 0;
byte temperature = 0;
byte humidity = 0;
const int pinDHT11 = 5;
///////////////////////////////////////////////////////////////////////////Objekte/Klassen///////////////////////////////////////////////

SimpleDHT11 dht11(pinDHT11);
WiFiClient wifiClient;
PubSubClient client(wifiClient);
///////////////////////////////////////////////////////////////////////////Funktionen/////////////////////////////////////////////////////
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

  if (callTopic.equals(subTopic)) {
    Serial.println("Topic match");
    Serial.println((char)payload[0]);
    if((char)payload[0] == '1') {
      digitalWrite(Relay_1, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("High");  
  
    } else if ((char)payload[0] == '0'){
      digitalWrite(Relay_1, LOW);    // turn the LED off by making the voltage LOW
      Serial.println("Low");  
    }    
  }

  if (callTopic.equals(subTopic2)) {
    if((char)payload[0] == '0') {
      //outletOff(1);
    } else if ((char)payload[0] == '1'){
     //outletOn(1);
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
  
      client.subscribe(subTopic);
      //client.subscribe(subTopic2);
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

/////////////////////////////////////////////////////////////////// Setup    ///////////////////////////////////////////
/*
 * First function to run.
 */
void setup() 
{
  Serial.begin(115200);
 
  pinMode(Relay_1, OUTPUT);
   digitalWrite(Relay_1, LOW); 
  connectWifi();
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
  client.loop();
  long now;
  now = millis();
  if (now - last_readtemp_time > readtemp_delay){
    //Serial.println(now - last_readtemp_time);
    last_readtemp_time = now;
    readtemp();
    snprintf (msg, 75, "%d", temperature);
    client.publish("WZ/Sensor/Temp", msg);
    snprintf (msg, 75, "%d", humidity);
    client.publish("WZ/Sensor/Hum", msg);
    }
}
