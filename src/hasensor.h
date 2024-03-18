#ifndef __HASENSOR_H__
#define __HASENSOR_H__

#include "haentity.h"

class PubSubClient;
class HADevice;

class HASensor : public HAEntity {
    protected:

        static const char *component;
        bool dirty;

    public:
        HASensor(const char *unique_id,const char *name,HADevice& device);
        HASensor(const char *unique_id,const char *name);

        inline bool isDirty() {return this->dirty;};
        virtual void onConnect(PubSubClient * client);
        virtual void onReceivedTopic(PubSubClient *, byte* payload,
            unsigned int length) {;}
};

#endif