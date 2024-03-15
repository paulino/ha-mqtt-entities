#include <Arduino.h>
#include <PubSubClient.h>

#include <hasensortext.h>

HASensorText::HASensorText(const char *unique_id, const char *name,
    HADevice& device, int max_size) : HASensorText(unique_id,name,max_size)
{
    this->device = &device;
}

HASensorText::HASensorText(const char *unique_id, const char *name, int max_size) :
    HASensor(unique_id,name)
{
    this->maxSize = max_size;
    this->state = new char[max_size+1];
    this->state[0] = 0;
}

void HASensorText::sendState(PubSubClient * client) {
    dirty = false;
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getStateTopic(topic);
    strncpy(payload,this->state,this->maxSize);
    client->publish(topic,payload);
}

void HASensorText::setState(const char *text) {
    if(strcmp(this->state,text) == 0)
        return;
    dirty = true;
    strncpy(this->state,text,this->maxSize);
    state[this->maxSize] = 0;
    this->onStateChange();
}