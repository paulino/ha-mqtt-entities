/* ha-mqtt-entities library number slicer example


   This example shows how to initialize the select entity with a const char*[]
   array to save RAM

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
#define HA_DEVICE_ID            "example04"
#define HA_DEVICE_FRIENDLY_NAME "Example Select HA-MQTT"

#define OPTIONS_COUNT 3
const char *const select_options[OPTIONS_COUNT] PROGMEM = {
     "Option 1",
     "Option 2",
     "Option 3"
     };

HADevice ha_device = HADevice(HA_DEVICE_ID,HA_DEVICE_FRIENDLY_NAME,"1.0");
HASelect ha_select = HASelect("select04uid","Select",ha_device,OPTIONS_COUNT,select_options);


void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_select);
    HAMQTT.setCallback(ha_callback);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if(!HAMQTT.connected() &&
            !HAMQTT.connect("examples",MQTT_USER,MQTT_PASSWORD))
        delay(1000);
    HAMQTT.loop();
}

/* Callback from HA-MQTT entities. It is called when an entity changes its state.

    Entity can be NULL when the received topic is not related to any entity.
    This is useful to handle other topics with the same mqtt client.
*/
void ha_callback(HAEntity *entity, char *topic, byte *payload,
    unsigned int length) {

    if(entity == &ha_select)
        Serial.printf("Select state: %s\n",ha_select.getState());

}
