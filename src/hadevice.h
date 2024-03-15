#ifndef __HA_DEVICE_H__
#define __HA_DEVICE_H__

class HADevice {
    protected:
         const char *identifier;
         const char *name;
         const char *sw_version;

    public:
        HADevice(const  char *indentifier,  const char *name, const  char *sw_vesion);
        inline const char *getIdentifier() {return identifier;}
        inline const char *getName() {return name;}
        inline const char *getSwVersion() {return sw_version;}

};
#endif