#include "hadevice.h"

HADevice::HADevice( const char *indentifier,  const char *name, const char *sw_vesion) {
    this->identifier = indentifier;
    this->name = name;
    this->sw_version = sw_vesion;
}


