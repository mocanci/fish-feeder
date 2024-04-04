#include <Arduino.h>
// #include <Servo.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include <cstring>

using namespace std;

void callBack(char *topic, byte *payload, unsigned int length);

// Servo servo;
WiFiClient wifi;

const char *ssid{"canci2G"};
const char *pass{"tooDoo2e"};

IPAddress mqttServer(192, 168, 1, 19);
PubSubClient mqttClient(mqttServer, 1883, callBack, wifi);
// const char *mqttServerAddress{"192.168.1.19"};

int MQTT_CONNECT_TIMEOUT = 20;
int WIFI_CONNECTION_TIMEOUT = 200;

const char *MQTT_CLIENT_ID = "Fish Tank Feeder";
const char *MQTT_ADVERTISE = "DENEME";
// const char* MQTT_ADVERTISE = "{"+
//   "\"icon\": \"mdi:memory\","+
//   "\"availability_topic\": \"fish-feeder/history/a\","+
//   "\"state_topic\": \"fish-feeder/history/s\","+
//   "\"name\": \"Last feed time\","+
//   "\"unique_id\": \"tank-fish-feeder\","+
//   "\"payload_available\": \"ON\","+
//   "\"payload_not_available\": \"OFF\","+
//   "\"device\": {"+
//     "\"identifiers\": ["+
//       "\"tank-fish-feeder\""+
//     "],"+
//     "\"name\": \"Tank Fish Feeder\","+
//     "\"model\": \"V1\","+
//     "\"manufacturer\": \"CANCI\""+
//   "}"+
// "}";

void setup()
{
  Serial.begin(9600);
  Serial.println("Setting up");
  WiFi.mode(WIFI_STA);
  WiFi.begin("canci2G", "tooDoo2e");

  while (WiFi.status() != WL_CONNECTED && WIFI_CONNECTION_TIMEOUT > 0)
  {
    delay(4000);
    WIFI_CONNECTION_TIMEOUT--;
    Serial.println("Waiting wifi connection...");
  }

  Serial.println("Wifi connected");
  Serial.print("IP addr:");
  Serial.println(WiFi.localIP());

  // servo.attach(5);
  Serial.println("Connecting to mqtt server");
  // mqttClient.setServer(mqttServerAddress, 1883);
  // mqttClient.setCallback(callBack);
  // Serial.println("Mqtt parameters are set!");

  while (!mqttClient.connected() && MQTT_CONNECT_TIMEOUT > 0)
  {
    Serial.println("Connecting... ");
    mqttClient.connect(MQTT_CLIENT_ID, "canci", "Onur5758");
    MQTT_CONNECT_TIMEOUT--;
    delay(5000);
  }

  if (mqttClient.connected())
  {
    mqttClient.setCallback(callBack);
    mqttClient.publish("homeassistant/sensor/fish-feeder/config", MQTT_ADVERTISE);
    mqttClient.subscribe("homeassistant/sensor/fish-feeder/action");
    Serial.println("Connected to mqtt server!");
  }
  else
  {
    Serial.println("Can't connect to mqtt server");
  }
}

void callBack(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void loop()
{

  // mqttClient.publish("FishTankFeeder", "Heartbeat");
  mqttClient.loop();
}
