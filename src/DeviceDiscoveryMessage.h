#include <MqttMessage.h>


class DeviceDiscoveryMessage : public Msg
{
private:
    /* data */
public:
    DeviceDiscoveryMessage(std::string deviceID);
    ~DeviceDiscoveryMessage();
};
