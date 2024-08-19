#include "hadevice.h"
#include "hamqttcontroller.h"

#include <string.h>
#include <stdio.h>
#include <Arduino.h>


const char *HADevice::configDeviceTemplate PROGMEM = "\
\"dev\":{\
\"ids\":\"%s\",\
\"name\":\"%s\",\
\"sw\":\"%s\"";

HADevice::HADevice(const char *identifier,  const char *name, 
    const char *sw_version, const char *manufacturer, const char *model,
    const char *hw_version ) {
    this->setIdentifier(identifier);
    this->setName(name);
    this->sw_version = sw_version;
    this->manufacturer = manufacturer;
    this->model = model;
    this->hw_version = hw_version;
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
}

void HADevice::setIdentifier(const char *identifier) {
    if (this->identifier != NULL) {
        delete this->identifier;
    }
    this->identifier = new char[strlen(identifier) + 1];
    sprintf(this->identifier, "%s", identifier);
}

void HADevice::setName(const char *name) {
    if (this->name != NULL) {
        delete this->name;
    }
    this->name = new char[strlen(name) + 1];
    sprintf(this->name, "%s", name);
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
    HAMQTTController::getInstance().setAvailable(available, *this);
}