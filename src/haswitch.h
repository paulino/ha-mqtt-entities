#ifndef __HASWITCH_H__
#define __HASWITCH_H__

#include "haentity.h"

class PubSubClient;
class HADevice;

class HASwitch : public HAEntity {
    protected:
        static const char *component;

        bool dirty;
        bool state;

    public:
        HASwitch(const char *unique_id,const char *name,HADevice& device);
        HASwitch(const char *unique_id,const char *name);

        inline bool getState() {return state;};
        inline bool isDirty() {return dirty;};
        void setState(bool state);

        void onConnect(PubSubClient * client);
        void onReceivedTopic(PubSubClient * client, byte *payload, unsigned int length);
        void sendState(PubSubClient * client);

};

#endif