/* ha-mqtt-entities library example of availability and last will for a device.

- Board: ESP32*

This library implements broker lastwill at device level sharing the availability
topic with all the entities of the device.

Note: The availability feature at entity level has precedence over the device
availability feature. If the device has the availability feature enabled, all
entities of the device must be disabled the availability feature.

*/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HaMqttEntities.h>

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
#define ENTITIES_COUNT 2
#define SW_VERSION PSTR("1.0.0")
#define IDENTIFIER PSTR("hamqen")
#define DEVICE_NAME PSTR("Example of availability and last will")

HADevice ha_device = HADevice(IDENTIFIER,DEVICE_NAME,SW_VERSION);
HASwitch ha_switch = HASwitch(
    PSTR("switch-id"), PSTR("My switch"), ha_device);
HASensorBinary ha_sensor = HASensorBinary(
    PSTR("sensor-id"), PSTR("My binary sensor"), ha_device);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup()
{
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    // Initialize entities
    HAMQTT.begin(mqtt_client, ENTITIES_COUNT);
    HAMQTT.addEntity(ha_switch);
    HAMQTT.addEntity(ha_sensor);

    // Features. The device availability enable last will at device level
    ha_device.addFeature(HA_FEATURE_AVAILABILITY);

    // By default the device is available is true but, it can be used in other
    // parts of the code to disable/enable the device if need

    // ha_device.setAvailable(true);

    // Initial states
    ha_switch.setState(true);

    HAMQTT.setCallback(ha_callback);

    // start wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("HaMqttEntities version: %s\n",HA_MQTT_VERSION_S);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED && !HAMQTT.connected())
    {
        // The connect method includes the last will in the pubsub client
        if (HAMQTT.connect("examples", MQTT_USER, MQTT_PASSWORD))
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
void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length)
{
    if (entity == &ha_switch)
        Serial.println("Switch changed");
}
