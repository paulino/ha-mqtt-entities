/* ha-mqtt-entities library number slicer example

- Board: ESP32*

This example creates an HA-MQTT device with a slicer-number entity.

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
HADevice ha_device = HADevice("example01","Example 1 HA-MQTTT","1.0");
HANumber ha_number = HANumber("example01number","Slicer",ha_device,1,100,1);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_number);
    ha_number.setState(50);
    HAMQTT.setCallback(ha_callback);

    // start wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if(WiFi.status() == WL_CONNECTED && !HAMQTT.connected()) {
         if( HAMQTT.connect("examples",MQTT_USER,MQTT_PASSWORD))
            Serial.println("Connected to MQTT");
         else
         {
            Serial.println("Failed to connect to MQTT");
            delay(5000);
         }
    }
    HAMQTT.loop();
    delay(50);
}

/* Callback from HA-MQTT entities. It is called when an entity changes its state.

    Entity can be NULL when the received topic is not related to any entity.
    This is useful to handle other topics with the same mqtt client.
*/
void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length){
    if(entity == &ha_number){
        Serial.printf("Changed number state to %d \n",ha_number.getState());
    }
    else
        Serial.println("Callback called from other subscription");

}
