// python script compatible
/* 
     o-o-o  o-o-o  o-o-o   o   o  o-o-o  o-o-o  o-o-o  o-o-o  o-o-o
       |    |-o    |       |-o-|  | o |    |    |-o-o    |      /
       |    o-o-o  o-o-o   o   o  o   o  o-o-o  o        |    o-o-o
       
*/

#include <ESP8266WiFi.h>

#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "127.0.0.1";

WiFiClient espClient;
PubSubClient client(espClient);
int FanPin = 16;
int LEDPin = 17;
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
  strTopic = String((char*)topic);
  if(strTopic == "ON")
    {
    device = String((char*)payload);
    if(device == "FAN")
      {
        Serial.println("Fan : ON");
        digitalWrite(FanPin, HIGH);
      }
    
    if(device == "LED")
      {
        Serial.println("LED : ON");
        digitalWrite(LEDPin, HIGH);
      }
    }

  if(strTopic == "OFF")
    {
    device = String((char*)payload);
    if(device == "FAN")
      {
        Serial.println("Fan : OFF");
        digitalWrite(FanPin, LOW);
      }
    
    if(device == "LED")
      {
        Serial.println("LED : OFF");
        digitalWrite(LEDPin, LOW);
      }
    }
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("ha/#");
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

  pinMode(FanPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
