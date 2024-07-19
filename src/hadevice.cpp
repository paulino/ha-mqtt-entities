#include "hadevice.h"
#include <string.h>
#include <stdio.h>


HADevice::HADevice( const char *identifier,  const char *name, const char *sw_version) {
    this->setIdentifier(identifier);
    this->setName(name);
    this->sw_version = sw_version;
}

HADevice::HADevice(const char *sw_version) {
    this->identifier = NULL;
    this->name = NULL;
    this->sw_version = sw_version;
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
