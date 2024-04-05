#pragma once

#include <ArduinoJson.h>
#include <PubSubClient.h>

class Msg
{
private:
protected:
    std::string topic;

public:
    Msg(std::string topic);
    ~Msg();

    JsonDocument payload;
    void publish(PubSubClient *client);
};