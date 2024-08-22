/* ha-mqtt-entities library example: later device configuration and multiple
   devices

- Board: ESP32*

This example show the follow:

- Multiple HA-devices in the same hardware device
- Delayed device configuration in the setup function
- Setting availability of one the device affected all entities of the device

Usage: Turning on/off the switch of device 1 change the availability of the
device 2.

*/

#include<Arduino.h>
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
#define SW_VERSION "1.0.0"

#define DEVICE1_ID "hadev1"
#define DEVICE1_NAME "Example: Device 1"

#define DEVICE2_ID "hadev2"
#define DEVICE2_NAME "Example: Device 2"

// Device 1 definition
HADevice ha_device_1 = HADevice(SW_VERSION);
HASwitch ha_switch_1 = HASwitch("switch","Switch",ha_device_1);

// Device 2 definition
HADevice ha_device_2 = HADevice(SW_VERSION);
HANumber ha_number_2 = HANumber("number","Slicer",ha_device_2,1,100,1);
HAText ha_text_2 = HAText("text","Input text",ha_device_2,100);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    // Delayed device configuration
    ha_device_1.setIdentifier(DEVICE1_ID);
    ha_device_1.setName(DEVICE1_NAME);

    ha_device_2.setIdentifier(DEVICE2_ID);
    ha_device_2.setName(DEVICE2_NAME);
    
    ha_number_2.addFeature(HA_FEATURE_AVAILABILITY);
    ha_text_2.addFeature(HA_FEATURE_AVAILABILITY);
    
    // Initial states
    ha_number_2.setAvailable(true);
    ha_text_2.setAvailable(true);

    ha_switch_1.setState(true);
    ha_number_2.setState(90);
    ha_text_2.setState("Hello HomeAssistant");

    // Initialize HA-MQTT
    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_switch_1);
    HAMQTT.addEntity(ha_number_2);
    HAMQTT.addEntity(ha_text_2);

    HAMQTT.setCallback(ha_callback);

    // start wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if(WiFi.status() == WL_CONNECTED && !mqtt_client.connected()) {
         if( mqtt_client.connect("examples",MQTT_USER,MQTT_PASSWORD))
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
void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length) {
    if(entity == &ha_switch_1) {
        if(ha_switch_1.getState()) {            
            ha_device_2.setAvailable(true);
        } else {
            ha_device_2.setAvailable(false);
        }
        
    }
}
