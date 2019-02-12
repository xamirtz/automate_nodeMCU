// python script compatible

#include <ESP8266WiFi.h>

#include <PubSubClient.h>

// #define LED D4

// Update these with values suitable for your network.
const char* ssid = "Techmirtz";
const char* password = "arduino123";
const char* mqtt_server = "192.168.43.146";

WiFiClient espClient;
PubSubClient client(espClient);
int FAN = 16;
int LED = 17;
String fan;
String led;
String strTopic;
String strPayload;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  Serial.println("Getting callback...");
  strTopic = String((char*)topic);
  Serial.print("Topic: ");
  Serial.println(strTopic);
  if(strTopic == "ON")
    {
    String device = String((char*)payload);
    if(device == "LED")
      {
        Serial.println("LED : ON");
        digitalWrite(LED, HIGH);
      }

    if(device == "FAN")
      {
        Serial.println("FAN : ON");
        digitalWrite(FAN, HIGH);
      }
    
    }

  if(strTopic == "OFF")
    {
    String device = String((char*)payload);
    
    if(device == "LED")
      {
        Serial.println("LED : OFF");
        digitalWrite(LED, LOW);
      }

    if(device == "FAN")
      {
        Serial.println("FAN : OFF");
        digitalWrite(FAN, LOW);
      }
    
    }
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("ON");
      client.subscribe("OFF");
      Serial.println("Topic Subscribed");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(FAN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  //Serial.println("Going to client.loop()");
  client.loop();
  //Serial.println("Exiting to client.loop()");
}
