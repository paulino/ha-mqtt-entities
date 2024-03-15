#ifndef __HASENSORTEXT__
#define __HASENSORTEXT__

#include "hasensor.h"

class PubSubClient;
class HADevice;

class HASensorText : public HASensor {
    protected:

        char *state;
        int maxSize;

    public:
        HASensorText(const char *unique_id,const char *name,HADevice& device,int max_size);
        HASensorText(const char *unique_id,const char *name,int max_size);

        const char *getState() {return this->state;};
        void setState(const char *state);
        void onReceivedTopic(PubSubClient * client, byte *payload, unsigned int length) {;}

        void sendState(PubSubClient * client);
};

#endif