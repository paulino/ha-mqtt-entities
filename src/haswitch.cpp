#include"haswitch.h"
#include<stdio.h>
#include<string.h>
#include <Arduino.h>
#include <PubSubClient.h>

#include "hadevice.h"

const char *const HASwitch::component PROGMEM = "switch";

HASwitch::HASwitch(const char *unique_id, const char *name, HADevice& device):
    HASwitch(unique_id,name) {
        this->device = &device;
}

HASwitch::HASwitch(const char *unique_id, const char *name):
    HAEntity(unique_id,name,component) {
        this->dirty = false;
        this->state = false;

}

// send config and send state
void HASwitch::onConnect(PubSubClient * client){
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getCommandTopic(topic);
    client->subscribe(topic);

    getConfigTopic(topic);
    getConfigPayload(payload,true,true);
    client->publish(topic,payload);
}

void HASwitch::sendState(PubSubClient * client){
    dirty = false;
    char topic[HA_MAX_TOPIC_LENGTH];
    getStateTopic(topic);
    if (this->state)
        client->publish(topic,"ON");
    else
        client->publish(topic,"OFF");
}

void HASwitch::setState(bool state){
    if (state == this->state)
        return;
    dirty = true;
    this->state = state;
    this->onStateChange();
}


void HASwitch::onReceivedTopic(PubSubClient * client, byte *payload,
    unsigned int length)
    {
    // Default values in homeassistant are "ON" and "OFF
    if (length < 1 || length > 3)
        return;
    if (payload[0]== 'O' && payload[1] == 'N')
        this->setState(true);
    else if (payload[0]== 'O' && payload[1] == 'F')
        this->setState(false);
}