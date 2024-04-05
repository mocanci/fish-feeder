#include <DeviceDiscoveryMessage.h>


DeviceDiscoveryMessage::DeviceDiscoveryMessage(std::string deviceID) : Msg(deviceID){
    topic ="homeassistant/sensor/"+deviceID+"/config";
    
    
    payload["unique_id"] = deviceID;
    payload["icon"] = "mdi:memory";
    payload["availability_topic"] = "fish-feeder/"+deviceID+"/availbility";
    payload["state_topic"] = "fish-feeder/"+deviceID+"/status";
    payload["action_topic"] = "fish-feeder/"+deviceID+"/action";
    payload["name"] = "Fish Tank Feeder";
    
    payload["payload_available"] = "ON";
    payload["payload_not_available"] = "OFF";
    payload["model"] = "V1";
    payload["manufacturer"] = "CANCI";
}

DeviceDiscoveryMessage::~DeviceDiscoveryMessage()
{
}