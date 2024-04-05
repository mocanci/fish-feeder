#include <StatusMessage.h>

StatusMessage::StatusMessage(std::string deviceID) : Msg("fish-feeder/"+deviceID+"/status")
{
    this->payload["status"] = "?";
    this->payload["time"] = millis();
}

StatusMessage::~StatusMessage()
{
}