#include"hanumber.h"
#include<stdio.h>
#include<string.h>
#include <Arduino.h>
#include <PubSubClient.h>

const char *HANumber::component PROGMEM  = "number";

HANumber::HANumber(const char *unique_id, const char *name,
    HADevice& device,int min,int max,const int step):
    HANumber(unique_id,name,min,max,step) {
        this->device = &device;
}

HANumber::HANumber(const char *unique_id, const char *name,
    int min,int max,const int step):
    HAEntity(unique_id,name,NULL,component) {
        this->dirty = false;
        this->max = max;
        this->min = min;
        this->step = step;
        this->state = min;
}

void HANumber::onConnect(PubSubClient * client){
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getCommandTopic(topic);
    client->subscribe(topic);

    getConfigTopic(topic);
    getConfigPayload(payload,true,true);
    sprintf(payload+strlen(payload)-1,",\"min\":%d,\"max\":%d,\"step\":%d}",
        this->min,this->max,this->step);
    client->publish(topic,payload);
}

void HANumber::sendState(PubSubClient * client){
    dirty = false;
    char payload[10];
    char topic[HA_MAX_TOPIC_LENGTH];
    getStateTopic(topic);
    sprintf(payload,"%d",this->state);
    client->publish(topic,payload);
}

void HANumber::setState(int state){
    if (state == this->state)
        return;
    dirty = true;
    if (state > this->max) state = this->max;
    if (state < this->min) state = this->min;
    this->state = state;
    this->onStateChange();
}

void HANumber::onReceivedTopic(PubSubClient * client,
uint8_t *payload, unsigned int length){
    char buff[15];
    if (length < 1 || length > 14)
        return;
    strncpy(buff,(char *)payload,length);
    this->setState(atoi(buff));
    this->sendState(client);
}
