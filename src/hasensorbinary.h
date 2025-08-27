#ifndef __HASENSORBINARY_H__
#define __HASENSORBINARY_H__

#include "hasensor.h"

class PubSubClient;
class HADevice;

class HASensorBinary : public HASensor {
    protected:
        static const char *const component;
        bool state;

    public:
        HASensorBinary(const char *unique_id,const char *name);
        HASensorBinary(const char *unique_id,const char *name,HADevice& device);

        bool getState() {return this->state;};
        void setState(bool state);

        void sendState(PubSubClient * client);
};

#endif