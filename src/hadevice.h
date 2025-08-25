#ifndef __HA_DEVICE_H__
#define __HA_DEVICE_H__

#include <stddef.h>
#include <stdint.h>

#include "haconsts.h"

class PubSubClient;

class HADevice {
    protected:
        static const char *const configDeviceTemplate;
        static const char *const availabilityTopicTemplate;

        char *identifier;
        char *name;
        const char *sw_version;
        const char *hw_version;
        const char *manufacturer;
        const char *model;

        int8_t available;

        HADevice();

    public:
        HADevice(const char *identifier, const char *name,
            const char *sw_version, const char *manufacturer = NULL,
            const char *model = NULL, const char *hw_version = NULL);

        HADevice(const char *sw_version);

        inline const char *getSwVersion() {return sw_version;}

        void setIdentifier(const char *identifier);
        inline const char *getIdentifier() {return identifier;}
        void setName(const char *name);
        // Not use dynamic or shared buffers, string is not copied
        inline void setManufacturer(const char *mf) {this->manufacturer = mf;}
        inline void setModel(const char *model) {this->model = model;}
        inline void setHwVersion(const char *hw_version)
            {this->hw_version = hw_version;}

        /// Add others predefined feature to the device
        void addFeature(int key, const char *value = NULL);
        char *getConfigPayload(char *buffer);

        /// Set the available state of all entities of the device
        void setAvailable(bool available);
        /// Returns true if the availability feature of the device is enabled
        inline bool getAvailability() {return available != HA_AVTY_DISABLED;}
        char *getAvailabilityTopic(char *buffer);
        void sendAvailable(PubSubClient *mqttClient,bool force);



};
#endif