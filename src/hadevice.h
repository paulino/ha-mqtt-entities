#ifndef __HA_DEVICE_H__
#define __HA_DEVICE_H__

class HADevice {
    protected:
         char *identifier;
         char *name;
         const char *sw_version;

    public:
        HADevice(const  char *identifier,  const char *name, const  char *sw_version);
        HADevice(const  char *sw_version);
        inline const char *getIdentifier() {return identifier;}
        inline const char *getName() {return name;}
        inline const char *getSwVersion() {return sw_version;}
        void setIdentifier(const char *identifier);
        void setName(const char *name);

};
#endif