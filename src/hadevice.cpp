#include "hadevice.h"
#include "hamqttcontroller.h"
#include "haconsts.h"

#include <string.h>
#include <stdio.h>
#include <Arduino.h>


const char *HADevice::configDeviceTemplate PROGMEM = "\
\"dev\":{\
\"ids\":\"%s\",\
\"name\":\"%s\",\
\"sw\":\"%s\"";


// Available topic shared by all entities
const char *HADevice::availabilityTopicTemplate PROGMEM = \
    HA_TOPIC_HEAD"/%s/available";


HADevice::HADevice(const char *identifier,  const char *name,
    const char *sw_version, const char *manufacturer, const char *model,
    const char *hw_version ) {
    this->identifier = NULL;
    this->name = NULL;
    this->sw_version = sw_version;
    this->manufacturer = manufacturer;
    this->model = model;
    this->hw_version = hw_version;
    this->available = HA_AVTY_DISABLED;
    this->setIdentifier(identifier);
    this->setName(name);
}

HADevice::HADevice(const char *sw_version) : HADevice() {
    this->sw_version = sw_version;
}
HADevice::HADevice() {
    this->identifier = NULL;
    this->name = NULL;
    this->manufacturer = NULL;
    this->model = NULL;
    this->hw_version = NULL;
    this->available = HA_AVTY_DISABLED;
}

void HADevice::setIdentifier(const char *identifier) {
    if (this->identifier != NULL)
        delete this->identifier;
    this->identifier = strdup(identifier);
}

void HADevice::setName(const char *name) {
    if (this->name != NULL)
        delete this->name;
    this->name = strdup(name);
}

char * HADevice::getConfigPayload(char *buffer) {
    int len;
    sprintf(buffer, configDeviceTemplate,
        this->identifier, this->name, this->sw_version);
    if (this->manufacturer != NULL) {
        len = strlen(buffer);
        sprintf(buffer + len, ",\"mf\":\"%s\"", this->manufacturer);
    }
    if (this->model != NULL) {
        len = strlen(buffer);
        sprintf(buffer + len, ",\"mdl\":\"%s\"", this->model);
    }
    if (this->hw_version != NULL) {
        len = strlen(buffer);
        sprintf(buffer + len, ",\"hw\":\"%s\"", this->hw_version);
    }
    len = strlen(buffer);
    buffer[len++] = '}';
    buffer[len] = '\0';
    return buffer;
}

void HADevice::setAvailable(bool available) {
    if (this->available != HA_AVTY_DISABLED)
    {
        if (available && this->available != HA_AVTY_ON)
            this->available = HA_AVTY_PENDING_ON;
        else if (!available && this->available != HA_AVTY_OFF)
            this->available = HA_AVTY_PENDING_OFF;
    }
    HAMQTTController::getInstance().setAvailable(available, *this);
}

void HADevice::addFeature(int key, const char *value) {
    if (key == HA_FEATURE_AVAILABILITY)
        this->available = HA_AVTY_PENDING_ON;

}

char *HADevice::getAvailabilityTopic(char *buffer)
{
    sprintf(buffer,availabilityTopicTemplate,this->identifier);
    return buffer;
}

void HADevice::sendAvailable(PubSubClient *mqttClient,bool force) {
    // TODO: Create a new class called HAAvalability
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
    getAvailabilityTopic(topic);
    if (this->available == HA_AVTY_PENDING_ON)
    {
        if (! mqttClient->publish(topic,"online"))
            return;
        this->available = HA_AVTY_ON;
    }
    else if (this->available == HA_AVTY_PENDING_OFF)
    {
        if (! mqttClient->publish(topic,"offline"))
            return;
        this->available = HA_AVTY_OFF;
    }
}