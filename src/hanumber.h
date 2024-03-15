#ifndef __HANUMBER_H__
#define __HANUMBER_H__

#include "haentity.h"

class PubSubClient;
class HADevice;

class HANumber : public HAEntity {
    protected:
        static const char *component;

        bool dirty;
        int state;
        int max;
        int min;
        int step;

    public:
        HANumber(const char *unique_id, const char *name,
            HADevice& device, int min, int max, int step);
        HANumber(const char *unique_id, const char *name, int min, int max,
            int step);

        inline int getState() {return this->state;};
        inline bool isDirty() {return this->dirty;};
        void setState(int state);

        void onConnect(PubSubClient * client);
        void onReceivedTopic(PubSubClient * client, byte *payload,
            unsigned int length);
        void sendState(PubSubClient * client);


};

#endif