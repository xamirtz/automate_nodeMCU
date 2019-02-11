#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#define RELAY D2 // pin labelled d2

// Update these with values suitable for your network.
const char* ssid = "techmirtz";
const char* password = "arduino123";
const char* host = "test"; // the name of your fixture, and the base channel to listen to
IPAddress MQTTserver(127, 0, 0, 1); // MQTT broker

// NO NEED TO EDIT FURTHER BEYOND HERE


#define BUFFER_SIZE 100
WiFiClient wclient;
PubSubClient client(wclient, MQTTserver);

void callback(const MQTT::Publish& pub) {

  uint16_t i, j;
  
  String myMessage = String(pub.payload_string());
  // handle message arrived
  Serial.print(pub.topic());
  Serial.print(" => ");
  String myTopic = String(pub.topic());
  
  
  if(myTopic == host)
  {

    Serial.println(pub.payload_string());

      if(pub.payload_string() == "on")
      {
        digitalWrite(RELAY,HIGH); 
        Serial.println("Turning on");
      }
      else
      {
        digitalWrite(RELAY, LOW);
        Serial.println("Turning off");
      }
   }

}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  pinMode(RELAY,OUTPUT);

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT callback
  client.set_callback(callback);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("ESP8266: Fountain")) {
        client.publish("outTopic",(String)"hello world, I'm "+host);
        client.subscribe(host+(String)"/#");
      }
    }

    if (client.connected())
      client.loop();
  }
  
}
