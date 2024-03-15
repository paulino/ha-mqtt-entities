#ifndef __HATEXT_H__
#define __HATEXT_H__

#include "haentity.h"

class PubSubClient;
class HADevice;

class HAText : public HAEntity {
    protected:

        static const char *component;

        bool dirty;
        char *state;
        unsigned int maxSize;

    public:
        HAText(const char *unique_id,const char *name,HADevice& device,
            unsigned int max_size);
        HAText(const char *unique_id,const char *name, unsigned int max_size);


        inline bool isDirty() {return this->dirty;};
        const char * getState() {return this->state;};
        void setState(const char *state);

        void onConnect(PubSubClient * client);
        void onReceivedTopic(PubSubClient * client, byte *payload, unsigned int length);
        void sendState(PubSubClient * client);

};

#endif