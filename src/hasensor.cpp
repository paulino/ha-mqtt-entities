#include <Arduino.h>
#include <PubSubClient.h>
#include <hasensor.h>

const char *const HASensor::component PROGMEM = "sensor";

HASensor::HASensor(const char *unique_id, const char *name,
    const char * component) :  HAEntity(unique_id,name,component) {

}

void HASensor::onConnect(PubSubClient * client){
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getConfigTopic(topic);
    getConfigPayload(payload,false,true);
    client->publish(topic,payload);
}

