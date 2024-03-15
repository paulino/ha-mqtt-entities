/* mqtt-ha-entities library switch example

    This example creates an HA-MQTT device with a switch entity.

*/
#include <Arduino.h>
#include<WiFi.h>
#include<PubSubClient.h>
#include<HaMqttEntities.h>

// This file is not included in the repository only used for local testing
// #include "secrets.h"

// You must set the next defines
#ifndef SECRETS_H
#define WIFI_SSID "MyWifi"
#define WIFI_PASSWORD "MyPassword"
#define MQTT_SERVER "192.168.1.X"
#define MQTT_PORT 1883
#define MQTT_USER "MyBrokerUser"
#define MQTT_PASSWORD "MyBrokerPassword"
#endif

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// HA Parts
#define ENTITIES_COUNT 1
HADevice ha_device("example02","Example 2 HA-MQTT","1.0");
HASwitch ha_switch = HASwitch("example02switch","Test on/off",ha_device);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_switch);
    HAMQTT.setCallback(ha_callback);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if(WiFi.status() == WL_CONNECTED && !mqtt_client.connected()) {
         if( mqtt_client.connect("examples",MQTT_USER,MQTT_PASSWORD))
            Serial.println("Connected to MQTT");
         else
            Serial.println("Failed to connect to MQTT");
    }
    HAMQTT.loop();
}

/* Callback from HA-MQTT entities. It is called when an entity changes its state.

    Entity can be NULL when the received topic is not related to any entity.
    This is useful to handle other topics with the same mqtt client.
*/
void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length){
    Serial.printf("Received topic: %s\n",topic);
    if(entity == &ha_switch){
        Serial.printf("Changed switch state to %d \n",ha_switch.getState());
    }

}
