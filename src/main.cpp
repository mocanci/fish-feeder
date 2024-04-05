#include <Arduino.h>
// #include <Servo.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include <cstring>
#include <EEPROM.h>
#include <Configuration.h>
#include "DeviceDiscoveryMessage.h"
#include "StatusMessage.h"

void callBack(char *topic, byte *payload, unsigned int length);

// Servo servo;
WiFiClient wifi;
PubSubClient *mqttClient{nullptr};


DeviceDiscoveryMessage *deviceBroadcast {nullptr};
StatusMessage *statusMsg {nullptr};
Configuration config;

void setup()
{
  EEPROM.begin(512);
  if (EEPROM.read(0) != 0)
  { // Configuration is set to EEPROM, read from EEPROM
    EEPROM.get(0, config);
  }

  deviceBroadcast = new DeviceDiscoveryMessage(config.deviceID);
  statusMsg = new StatusMessage(config.deviceID);

  Serial.begin(9600);

  Serial.println("Setting up");
  WiFi.mode(config.isAP ? WIFI_AP_STA : WIFI_STA);
  WiFi.begin(config.ssid.c_str(), config.password.c_str());

  int WIFI_CONNECTION_TIMEOUT = 30;
  while (WiFi.status() != WL_CONNECTED && WIFI_CONNECTION_TIMEOUT > 0)
  {
    delay(4000);
    WIFI_CONNECTION_TIMEOUT--;
    Serial.println("Waiting wifi connection...");
  }

  Serial.print("Wifi connected IP addr:");
  Serial.println(WiFi.localIP());

  // servo.attach(5);
  Serial.println("Connecting to mqtt server");
  mqttClient = new PubSubClient(config.mqttServer, 1883, wifi);

  int MQTT_CONNECT_TIMEOUT = 30;
  while (!(*mqttClient).connected() && MQTT_CONNECT_TIMEOUT > 0)
  {
    Serial.println("Connecting... ");
    (*mqttClient).connect(config.deviceID.c_str(), config.mqttUser.c_str(), config.mqttPass.c_str());
    MQTT_CONNECT_TIMEOUT--;
    delay(5000);
  }

  if ((*mqttClient).connected())
  {
    mqttClient->setCallback(callBack);
    mqttClient->subscribe(("fish-feeder/"+config.deviceID+"/action").c_str());
    deviceBroadcast->publish(mqttClient);
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
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void loop()
{
  deviceBroadcast->publish(mqttClient);
  mqttClient->loop();
}
