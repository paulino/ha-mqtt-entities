#include <Arduino.h>
#include <PubSubClient.h>

#include <hasensornumeric.h>


HASensorNumeric::HASensorNumeric(const char *unique_id, const char *name,
    HADevice& device, const char* unit_of_measurement, int precision) :
    HASensorNumeric(unique_id,name,unit_of_measurement,precision)
{
    this->device = &device;

}
HASensorNumeric::HASensorNumeric(const char *unique_id, const char *name,
    const char* unit_of_measurement, int precision) : HASensor(unique_id,name)
{
    this->unitOfMeasurement = unit_of_measurement;
    this->state = 0;
    if (precision < 0)
        this->precision = 0;
    else if (precision > 9)
        this->precision = 9;
    else
        this->precision = precision;
}


void HASensorNumeric::onConnect(PubSubClient * client){
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getConfigTopic(topic);
    getConfigPayload(payload,false,true);
    if(this->unitOfMeasurement != NULL){
        int len = strlen(payload);
        sprintf(payload + len - 1 ,",\"unit_of_measurement\":\"%s\"}",this->unitOfMeasurement);
    }
    client->publish(topic,payload);
}


void HASensorNumeric::sendState(PubSubClient * client)
{
    dirty = false;
    char topic[HA_MAX_TOPIC_LENGTH];
    char payload[10];
    char format[10];
    getStateTopic(topic);
    sprintf(format,"%%.%df",this->precision);
    sprintf(payload,format,this->state);
    client->publish(topic,payload);
}

void HASensorNumeric::setState(float n)
{
    if(this->state == n)
        return;
    dirty = true;
    this->state = n;
    this->onStateChange();
}