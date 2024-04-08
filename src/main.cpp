#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include <cstring>
#include <EEPROM.h>
#include <Configuration.h>
#include "DeviceDiscoveryMessage.h"
#include "StatusMessage.h"
#include "ESP8266_ISR_Servo.h"
#include <string>
#include <iostream>

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS 800 // 544
#define MAX_MICROS 2450

void callBack(char *topic, byte *payload, unsigned int length);

WiFiClient wifi;
PubSubClient *mqttClient{nullptr};

DeviceDiscoveryMessage *deviceBroadcast{nullptr};
StatusMessage *statusMsg{nullptr};
Configuration config;

int servo{0}, position{0};

void setup()
{
  Serial.begin(9600);

  EEPROM.begin(512);
  // if (EEPROM.read(0) != 0)
  // { // Configuration is set to EEPROM, read from EEPROM
  //   EEPROM.get(0, config);

  //   Serial.print("DeviceID:");
  //   Serial.print(config.deviceID.c_str());
  // }
  // else
  // {
  // config.deviceID = "FishFeeder";
  // config.isAP = false;
  // config.mqttPass = "onur57";
  // config.mqttServer = IPAddress(192, 168, 1, 19);
  // config.mqttUser = "canci";
  // config.password = "tooDoo2e";
  // config.ssid = "canci2G";

  EEPROM.put(0, config);
  // }

  servo = ISR_Servo.setupServo(D2, MIN_MICROS, MAX_MICROS);
  ISR_Servo.setPosition(servo, 0);

  deviceBroadcast = new DeviceDiscoveryMessage(config.deviceID);
  statusMsg = new StatusMessage(config.deviceID);

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
    Serial.print("Connecting... ");
    Serial.print(config.mqttUser.c_str());
    Serial.print(":");
    Serial.print(config.mqttPass.c_str());

    (*mqttClient).connect(config.deviceID.c_str(), config.mqttUser.c_str(), config.mqttPass.c_str());
    MQTT_CONNECT_TIMEOUT--;
    delay(5000);
  }

  if ((*mqttClient).connected())
  {
    mqttClient->setCallback(callBack);
    mqttClient->subscribe(("fish-feeder/" + config.deviceID + "/action").c_str());
    mqttClient->subscribe("feeder-pos");
    deviceBroadcast->publish(mqttClient);
    Serial.println("Connected to mqtt server!");
  }
  else
  {
    Serial.println("Can't connect to mqtt server");
  }
}

JsonDocument mqttCommandDocument;
void callBack(char *topic, byte *payload, unsigned int length)
{

  Serial.println(topic);

  if (strcmp(topic, "feeder-pos") == 0)
  {
    Serial.println("Feeding command received");
    deserializeJson(mqttCommandDocument, payload);
    String a = mqttCommandDocument["position"].as<String>();

    ISR_Servo.setPosition(servo, a.toInt());
  }

  /* Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println(); */
}

char serialInput[15];

void loop()
{
  // deviceBroadcast->publish(mqttClient);

  mqttClient->loop();
  delay(1000);

  // Serial.readBytesUntil('\n',serialInput,4);

  // ISR_Servo.setPosition(servo,position);
  // position = (position+30)%180;
  // delay(5000);
}
