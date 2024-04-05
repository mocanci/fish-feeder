#include <MqttMessage.h>

class StatusMessage : public Msg
{
private:
    /* data */
public:
    StatusMessage(std::string deviceID);
    ~StatusMessage();
};
