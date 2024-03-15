/*
    Warning:  Not use ids (unique_id,device_id) with more than 50 characters
*/
#ifndef __HAENTITY_H__
#define __HAENTITY_H__

#include <Arduino.h>
class PubSubClient;
class HADevice;

#ifndef HA_MAX_TOPIC_LENGTH
#define HA_MAX_TOPIC_LENGTH 128
#endif
#ifndef HA_MAX_PAYLOAD_LENGTH
#define HA_MAX_PAYLOAD_LENGTH 512
#endif


/** Abstract class for HA entities

*/
class HAEntity
 {
    protected:

        static const char *configTopicTemplate;
        static const char *configDeviceTemplate;
        static const char *configCommandTopicTemplate;
        static const char *configStateTopicTemplate;
        static const char *configPayloadTemplate;

        static const char *commandTopicTemplate;
        static const char *stateTopicTemplate;

        HADevice *device;
        const char *uniqueId;
        const char *name;
        const char *component;

        char *getConfigTopic(char *buffer);
        char *getConfigPayload(char *buffer, bool add_command_topic,
            bool add_state_topic);

        // Build default state topic
        char *getStateTopic(char *buffer);


    public:
        HAEntity(const char *unique_id, const char *name,HADevice *device,
            const char *component);

        inline const char *getUniqueId() {return this->uniqueId;}

        /// Some entities do not have a command topic and returns NULL
        virtual char *getCommandTopic(char *buffer) ;
        virtual void sendState(PubSubClient * ) {;}
        virtual void setState()  {;}

        /// Flag used in HAMQTT indicating if the state has changed
        /// and needs to be sent to the broker
        virtual bool isDirty() {return false;}

        virtual void onConnect(PubSubClient * ) = 0;
        virtual void onReceivedTopic(PubSubClient *, byte* payload,
            unsigned int length) = 0;

        // Action to be performed when the state changes by default it does nothing
        // It can be redefined in the derived class
        virtual void onStateChange() {;}


};

#endif