/* ha-mqtt-entities library example of availability

- Board: ESP32*

This example shows the next:

- Creates a text sensor with availability feature
- Customize the icon of the test sensor
- Create a switch to control the availability of the text sensor
- Optimizes the memory usage by using PSTR to store the strings in flash memory

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
#define ENTITIES_COUNT 3
// This example optimizes the memory usage by using PSTR to store the strings 
// in flash memory
#define SW_VERSION PSTR("1.0.0")
#define IDENTIFIER PSTR("ha-mqtt-entities")
#define DEVICE_NAME PSTR("Example of availability")

HADevice ha_device = HADevice(IDENTIFIER,DEVICE_NAME,SW_VERSION);
HAText ha_text = HAText(PSTR("text"), PSTR("Input text"), ha_device, 100);
HASwitch ha_switch = HASwitch(PSTR("switch"), PSTR("Test availability"), ha_device);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup()
{
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    // Initialize entities
    HAMQTT.begin(mqtt_client, ENTITIES_COUNT);
    HAMQTT.addEntity(ha_text);
    HAMQTT.addEntity(ha_switch);

    ha_text.addFeature(HA_FEATURE_AVAILABILITY);
    ha_text.addFeature(HA_FEATURE_ICON,"mdi:lightbulb");

    // Initial states
    ha_switch.setState(true);
    ha_text.setAvailable(true);
    ha_text.setState("Hello HomeAssistant");
    
    HAMQTT.setCallback(ha_callback);

    // start wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED && !HAMQTT.connected())
    {
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
    {
        if (ha_switch.getState())
        {            
            ha_text.setAvailable(true);
        }
        else
        {
            ha_text.setAvailable(false);
        }
    }
}