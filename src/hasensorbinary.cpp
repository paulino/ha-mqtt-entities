#include <Arduino.h>
#include <PubSubClient.h>

#include <hasensorbinary.h>

const char *const HASensorBinary::component PROGMEM = "binary_sensor";

HASensorBinary::HASensorBinary(const char *unique_id, const char *name,
    HADevice& device) : HASensor(unique_id,name,component)
{
    this->device = &device;
    this->state = false;
}

HASensorBinary::HASensorBinary(const char *unique_id, const char *name)
    : HASensor(unique_id,name, component)
{
    this->state = false;
}

void HASensorBinary::sendState(PubSubClient * client)
{
    dirty = false;
    char topic[HA_MAX_TOPIC_LENGTH];
    getStateTopic(topic);
    if (this->state)
        client->publish(topic,"ON");
    else
        client->publish(topic,"OFF");
}

void HASensorBinary::setState(bool on_off)
{
    if(this->state == on_off)
        return;
    dirty = true;
    this->state = on_off;
    this->onStateChange();
}