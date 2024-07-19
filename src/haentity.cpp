#include"haentity.h"
#include"hadevice.h"

/* HA discovery template is
 <discovery_prefix>/<component>/[<node_id>/]<object_id>/config node_id is
 optional and will ignored in this implementation
 example:
    {
   "name":"Motion Sensor",
   "device_class":"motion",
   "state_topic":"homeassistant/binary_sensor/garden/state",
   "unique_id":"garden01mot",
   "device":{
      "identifiers":"garden01",
      "name":"Garden Surveillance",
    }
}
*/

// Configuration topic
const char *HAEntity::configTopicTemplate PROGMEM = "homeassistant/%s/%s/config";

// Configuration payload divided in 4 parts
const char *HAEntity::configPayloadTemplate PROGMEM = "{\
\"name\":\"%s\",\
\"unique_id\":\"%s\"";

const char *HAEntity::configDeviceTemplate PROGMEM = "\
\"device\":{\
\"identifiers\":\"%s\",\
\"name\":\"%s\",\
\"sw_version\":\"%s\"\
}";

const char *HAEntity::configCommandTopicTemplate PROGMEM =
    "\"command_topic\": \"homeassistant/%s/%s/set\"";

const char *HAEntity::configStateTopicTemplate PROGMEM =
    "\"state_topic\":\"homeassistant/%s/%s/state\"";

const char *HAEntity::commandTopicTemplate PROGMEM = "homeassistant/%s/%s/set";
const char *HAEntity::stateTopicTemplate  PROGMEM= "homeassistant/%s/%s/state";

HAEntity::HAEntity(const char *id,const char *name,HADevice* ha_device,
    const char *component) {
    this->uniqueId = NULL;
    this->id = id ;
    this->component = component;
    this->name = name;
    this->device = ha_device;
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

char * HAEntity::getConfigPayload(char *buffer,
        bool add_command_topic,bool add_state_topic) {
    int len;
    sprintf(buffer,configPayloadTemplate, name, getUniqueId());
    len = strlen(buffer);
    if (this->device != NULL)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1,configDeviceTemplate,
            device->getIdentifier(),
            device->getName(),
            device->getSwVersion());
        len = strlen(buffer);
    }
    if (add_command_topic)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1,configCommandTopicTemplate,component,getUniqueId());
        len = strlen(buffer);
    }
    if (add_state_topic)
    {
        buffer[len] = ',';
        sprintf(buffer+len+1,configStateTopicTemplate,component,getUniqueId());
        len = strlen(buffer);
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
