#include <string>
#include <ESP8266WiFi.h>

struct Configuration
{
    // char ssid[10] {};
    std::string ssid = "FishTankFeeder";
    bool isAP = true;
    int slots = 6;
    std::string deviceID = "FishTank";
    std::string password = "12345678";
    IPAddress mqttServer {127,0,0,1};
    std::string mqttUser = "user";
    std::string mqttPass = "pass";
    
};
