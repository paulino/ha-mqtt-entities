/*
    Warning:  Not use ids (unique_id,device_id) with more than 50 characters
*/
#ifndef __HAENTITY_H__
#define __HAENTITY_H__

#include <Arduino.h>
#include "hakvpairlist.h"

#define HA_FEATURE_AVAILABILITY -1
#define HA_FEATURE_DEVICE_CLASS 0
#define HA_FEATURE_ICON 1

#ifndef HA_MAX_TOPIC_LENGTH
#define HA_MAX_TOPIC_LENGTH 128
#endif
#ifndef HA_MAX_PAYLOAD_LENGTH
#define HA_MAX_PAYLOAD_LENGTH 512
#endif

class PubSubClient;
class HADevice;

/** Abstract class for HA entities

*/
class HAEntity
 {
    protected:

        static const char *configTopicTemplate;
        static const char *configPayloadTemplate;
        static const char *availabilityTopicTemplate;
        static const char *featureKeys[];

        static const char *commandTopicTemplate;
        static const char *stateTopicTemplate;

        HADevice *device;
        const char *id;
        char *uniqueId; /** Derived from id and device */
        const char *name;
        const char *component;
        HAKVPairList features;
        int8_t available;

        char *getConfigTopic(char *buffer);
        char *getConfigPayload(char *buffer, bool add_command_topic,
            bool add_state_topic);

        /// Build default state topic
        char *getStateTopic(char *buffer);


    public:
        HAEntity(const char *unique_id, const char *name, const char *component,
            HADevice *device = NULL);

        const char *getUniqueId();
        inline HADevice *getDevice() { return device; }

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

        /// Add predefined feature to the entity
        void addFeature(int key, const char *value = NULL);
        
        /// Add not predefined feature to the entity
        void addFeature(const char *key, const char *value);

        /// It only works if previously the availability feature has been added
        void setAvailable(bool available);
        void sendAvailability(PubSubClient *);

};

#endif