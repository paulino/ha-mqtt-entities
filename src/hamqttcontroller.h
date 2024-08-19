/*

In exclusive mode, the controller will subscribe to the topic of each component and will forward the messages to the corresponding component.
Any topic that is not recognized will be ignored.

In shared mode, the controller will subscribe to the topic of each component and will forward the  user callback
with a pointer to the component or NULL if the topic is not recognized.

*/
#ifndef __HAMQTTCONTROLLER_H__
#define __HAMQTTCONTROLLER_H__

#include "haentity.h"
#include <PubSubClient.h>


/* Callback signature:
    void callback(HAEntity*, char* topic, byte* payload, unsigned int payload_length)
*/
#define HAMQTT_CALLBACK_SIGNATURE(f_name) void (*f_name)(HAEntity*, char*, byte*, unsigned int)

// Singleton class associated to the pubsub MQTT client
class HAMQTTController {

    private:
        static HAMQTTController *instance;
        static const char *topicHass;
        HAMQTT_CALLBACK_SIGNATURE(callback);
        HAMQTTController();

        unsigned long int delaySendState;

    protected:
        PubSubClient *mqttClient;
        HAEntity **entities;
        int entityCounter;
        static void pubSubClientHandler(char* topic, byte* payload, unsigned int length);
        enum State {Connected, Disconnected } state ;

    public:
        static HAMQTTController& getInstance();
        HAMQTTController(const HAMQTTController&) = delete;
        HAMQTTController& operator=(const HAMQTTController&) = delete;

        void begin(PubSubClient& mqtt_client,int component_count);
        void addEntity(HAEntity& entity);

        /** Connect to the MQTT broker using the PubSubClient instance.
         *
         * On success, it calls onConnect. Using this method is optional,
         * but if it is not used, after connecting to MQTT, the onConnect
         * method must be called manually.
         */
        boolean connect(const char *id, const char *user, const char *pass);
        inline boolean connected() { return this->mqttClient->connected(); }
        void onConnect();
        void loop();
        void sendAllStates();

        // It must be called when a mqtt message is received
        // return true if the message is for one of the components
        bool mqttOnReceived(char *topic, byte *payload, unsigned int length);

        /**
         * Set the MQTT callback
         *
         * When is not set the messages are only processed in each
         * component in the mqttOnReceived method
         *
         * When is set, first the message is processed in each component
         * in the mqttOnReceived method and then the callback is called
         *
         */
        void setCallback(HAMQTT_CALLBACK_SIGNATURE(callback));

        void setAvailable(bool available, HADevice& device);

};

#endif