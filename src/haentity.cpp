#include"haentity.h"
#include"hadevice.h"

#include <PubSubClient.h>

/* HA discovery template is
 <discovery_prefix>/<component>/[<node_id>/]<object_id>/config node_id is
 optional and will ignored in this implementation
*/

#define HA_AVTY_OFF 0
#define HA_AVTY_ON 1
#define HA_AVTY_PENDING_ON 2
#define HA_AVTY_PENDING_OFF 3
#define HA_AVTY_DISABLED 4

// Configuration topic
const char *HAEntity::configTopicTemplate PROGMEM = "homeassistant/%s/%s/config";

// Configuration payload parts
const char *HAEntity::configPayloadTemplate PROGMEM = "{\
\"~\":\"homeassistant/%s/%s\",\
\"name\":\"%s\",\
\"uniq_id\":\"%s\"";

const char *HAEntity::commandTopicTemplate PROGMEM = "homeassistant/%s/%s/set";
const char *HAEntity::stateTopicTemplate PROGMEM = "homeassistant/%s/%s/state";
const char *HAEntity::availabilityTopicTemplate PROGMEM = 
    "homeassistant/%s/%s/available";


const char *HAEntity::featureKeys[] PROGMEM = {
    "dev_cla",
    "ic"
    };


HAEntity::HAEntity(const char *id,const char *name,const char *component,
    HADevice *ha_device) {
    this->uniqueId = NULL;
    this->id = id ;
    this->component = component;
    this->name = name;
    this->device = ha_device;
    this->available = HA_AVTY_DISABLED;
    this->features = NULL;
}

const char *HAEntity::getUniqueId() {
    if (this->device == NULL)
        return this->id;
    if (this->uniqueId == NULL)
    {
        this->uniqueId = new char[strlen(this->id) + strlen(this->device->getIdentifier()) + 2];
        sprintf(this->uniqueId,"%s%s",this->device->getIdentifier(),this->id);
    }
    return this->uniqueId;
}

char *HAEntity::getConfigPayload(char *buffer,
        bool add_command_topic,bool add_state_topic) {
    int len;
    sprintf(buffer,configPayloadTemplate, component,getUniqueId(), name, 
        getUniqueId());
    len = strlen(buffer);
    if (this->device != NULL)
    {
        buffer[len] = ',';
        this->device->getConfigPayload(buffer+len+1);
        len = strlen(buffer);
    }    
    if (add_command_topic)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1, PSTR("\"cmd_t\":\"~/set\""));
        len = strlen(buffer);
    }
    if (add_state_topic)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1, PSTR("\"stat_t\":\"~/state\""));
        len = strlen(buffer);
    }
    // Availability if set
    if (this->available != HA_AVTY_DISABLED)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1,PSTR("\"avty_t\":\"~/available\""));
        len = strlen(buffer);
    }
    // Add features
    HAKVPairList *pair = features;
    while(pair!=NULL && pair->getKey() != NULL)
    {
        buffer[len] = ',';
        if (pair->getKey() == featureKeys[HA_FEATURE_AVAILABILITY])
            sprintf(buffer+len+1,PSTR("\"%s\":\"available\""),pair->getKey());
        else
            sprintf(buffer+len+1,PSTR("\"%s\":\"%s\""),pair->getKey(),
                pair->getValue());
        len = strlen(buffer);
        pair = pair->getNext();
    }

    buffer[len] = '}';
    buffer[len+1] = '\0';
    return buffer;
}

char *HAEntity::getConfigTopic(char *buffer){
    sprintf(buffer,configTopicTemplate,this->component,getUniqueId());
    return buffer;
}

char *HAEntity::getCommandTopic(char *buffer) {
    sprintf(buffer,commandTopicTemplate,component,getUniqueId());
    return buffer;
}

char *HAEntity::getStateTopic(char *buffer) {
    sprintf(buffer,stateTopicTemplate,component,getUniqueId());
    return buffer;
}

void HAEntity::addFeature(int key, const char *value) {
    if (key == HA_FEATURE_AVAILABILITY)
        this->available = HA_AVTY_PENDING_ON;
    else
        features->append(featureKeys[key],value);
}

void HAEntity::addFeature(const char *key, const char *value) {
    if (this->features == NULL)
        this->features = new HAKVPairList(key,value);
    else
        features->append(key,value);
}

void HAEntity::setAvailable(bool available) {
    if (this->available == HA_AVTY_DISABLED)
        return;
    if (available && this->available != HA_AVTY_ON) 
        this->available = HA_AVTY_PENDING_ON;
    else if (!available && this->available != HA_AVTY_OFF)
        this->available = HA_AVTY_PENDING_OFF;
}

void HAEntity::sendAvailability(PubSubClient *mqttClient,bool force) {
    if(this->available == HA_AVTY_DISABLED)
        return;
    if (force && this->available == HA_AVTY_ON)
        this->available = HA_AVTY_PENDING_ON;
    else if (force && this->available == HA_AVTY_OFF)
        this->available = HA_AVTY_PENDING_OFF;
        
    if ( this->available != HA_AVTY_PENDING_ON &&
        this->available != HA_AVTY_PENDING_OFF)
        return;

    char topic[HA_MAX_TOPIC_LENGTH];
    char payload[HA_MAX_PAYLOAD_LENGTH];
    getAvailabilityTopic(topic);
    if (this->available == HA_AVTY_PENDING_ON)
    {
        sprintf(payload,PSTR("online"));
        if (! mqttClient->publish(topic,payload))
            return;
        this->available = HA_AVTY_ON;
    }
    else if (this->available == HA_AVTY_PENDING_OFF)
    {
        sprintf(payload,PSTR("offline"));
        if (! mqttClient->publish(topic,payload))
            return;
        this->available = HA_AVTY_OFF;
    }
}

char *HAEntity::getAvailabilityTopic(char *buffer) 
{
    sprintf(buffer,availabilityTopicTemplate,component,getUniqueId());
    return buffer;
}