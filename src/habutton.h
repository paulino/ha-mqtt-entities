#ifndef __HABUTTON_H__
#define __HABUTTON_H__

#include<haentity.h>

class PubSubClient;
class HADevice;

/**
 * @brief Button entity for Home Assistant
 *
 *  Button does not store the state, the event must be handled in the callback
 *  or in the derived class method onStateChange
 *
 *  HA send by default the string "PRESS", but this class not check the payload,
 *  it assumes that the payload is "PRESS"
 */

class HAButton : public HAEntity {
    protected:
        static const char *component;        

    public:
        HAButton(const char *unique_id,const char *name,HADevice& device);
        HAButton(const char *unique_id,const char *name);

        void onConnect(PubSubClient * client);
        void onReceivedTopic(PubSubClient * client,
            byte *payload, unsigned int length) {;}

};

#endif