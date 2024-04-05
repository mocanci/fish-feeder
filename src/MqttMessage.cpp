#include <MqttMessage.h>

Msg::Msg(std::string topic)
{
    this->topic = topic;
}

Msg::~Msg()
{
}

void Msg::publish(PubSubClient *client)
{
    String payloadMsg;
    serializeJson(payload, payloadMsg);

    client->publish(topic.c_str(), payloadMsg.c_str());
}
