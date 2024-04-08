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

    Serial.print("Sending broadcast message");
    // Serial.println(topic.c_str());
    // Serial.print(" : ");
    Serial.println(payloadMsg.c_str());
    client->publish(topic.c_str(), payloadMsg.c_str());
}
