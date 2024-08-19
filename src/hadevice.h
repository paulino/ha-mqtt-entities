#ifndef __HA_DEVICE_H__
#define __HA_DEVICE_H__

#include <cstddef>

class HADevice {
    protected:
        static const char *configDeviceTemplate;
       
        char *identifier;
        char *name;
        const char *sw_version;
        const char *hw_version;
        const char *manufacturer;
        const char *model;

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

        char *getConfigPayload(char *buffer);

        /// Set the availability of all entities of the device
        /// Only works for entities that have the availability feature enabled
        void setAvailable(bool available);

};
#endif