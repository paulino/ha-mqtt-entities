#ifndef __HASENSORNUMERIC_H__
#define __HASENSORNUMERIC_H__

#include "hasensor.h"

class PubSubClient;
class HADevice;

class HASensorNumeric : public HASensor {
    protected:

        float state;
        int precision;

    public:
        HASensorNumeric(const char *unique_id,const char *name,HADevice& device,int precision = 0);
        HASensorNumeric(const char *unique_id,const char *name,int precision = 0);


        float getState() {return this->state;};
        void setState(float state);

        void sendState(PubSubClient * client);
};

#endif