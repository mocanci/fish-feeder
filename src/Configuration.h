#include <string>
#include <ESP8266WiFi.h>

struct Configuration
{
    // char ssid[10] {};
    std::string ssid = "canci2G";
    bool isAP = false;
    int slots = 6;
    std::string deviceID = "FishTank";
    std::string password = "tooDoo2e";
    IPAddress mqttServer {192,168,1,19};
    std::string mqttUser = "canci";
    std::string mqttPass = "Onur5758";
    
};
