/* ha-mqtt-entities library example with multiple entities into a single device

- Board: ESP32*

This example creates an HA-MQTT device with a group of entities.
It also shows how to use the callback to handle events from the entities.

You must set the definitions in the code under SECRETS_H.

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
#define ENTITIES_COUNT 3
#define HA_DEV_ID   "example04"
#define HA_DEV_FRIENDLY_NAME "Example 4 HA-MQTT"
#define HA_DEV_FIRMWARE_VERSION "1.0"
#define HA_DEV_MANUFACTURER "Arduino"
#define HA_DEV_MODEL "ESP32"
#define HA_DEV_HARDWARE_VERSION "1.8.19"

HADevice ha_device = HADevice(HA_DEV_ID,HA_DEV_FRIENDLY_NAME,
    HA_DEV_FIRMWARE_VERSION,HA_DEV_MANUFACTURER,HA_DEV_MODEL,HA_DEV_HARDWARE_VERSION);
HAText ha_text = HAText("text04uid","Input text",ha_device,100);
HAButton ha_button = HAButton("button04uid","Calculate length",ha_device);
HASelect ha_select = HASelect("select04uid","Select",ha_device,3);
HASensorNumeric ha_sensor = HASensorNumeric("sensor04uid","Text length",ha_device);


void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_button);
    HAMQTT.addEntity(ha_sensor);
    HAMQTT.addEntity(ha_text);
    HAMQTT.addEntity(ha_select);

    ha_select.addOption("Option 1");
    ha_select.addOption("Option 2");

    HAMQTT.setCallback(ha_callback);

    ha_text.setState("Hello World");
    ha_sensor.setState(strlen(ha_text.getState()));

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if(WiFi.status() == WL_CONNECTED && !HAMQTT.connected())
    {
        Serial.println("Connecting to MQTT...");
        if (HAMQTT.connect("examples",MQTT_USER,MQTT_PASSWORD))
            Serial.println("Connected to MQTT");
        else {
            Serial.print("Failed to connect to MQTT, retry in 5 seconds...");
            delay(5000);
        }
    }
    HAMQTT.loop();
}

/* Callback from HA-MQTT entities. It is called when an entity changes its state.

    Entity can be NULL when the received topic is not related to any entity.
    This is useful to handle other topics with the same mqtt client.
*/
void ha_callback(HAEntity *entity, char *topic, byte *payload,
    unsigned int length) {
    if(entity == &ha_button)
        ha_sensor.setState(strlen(ha_text.getState()));
    if(entity == &ha_select)
        ha_text.setState(ha_select.getState());

}
