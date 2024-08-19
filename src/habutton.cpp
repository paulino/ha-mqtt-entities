#include <Arduino.h>
#include <PubSubClient.h>

#include<habutton.h>

const char *HAButton::component PROGMEM  = "button";

HAButton::HAButton(const char *unique_id, const char *name, HADevice& device):
    HAEntity(unique_id,name,component,&device) {
}
HAButton::HAButton(const char *unique_id, const char *name):
    HAEntity(unique_id,name,component) {
}

void HAButton::onConnect(PubSubClient * client){
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getCommandTopic(topic);
    client->subscribe(topic);

    getConfigTopic(topic);
    getConfigPayload(payload,true,false);
    client->publish(topic,payload);
}