/* ha-mqtt-entities library example with all device properties set.

- Board: ESP32*

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
#define ENTITIES_COUNT 1
#define SW_VERSION "1.0.0"
HADevice ha_device = HADevice(SW_VERSION);
HANumber ha_number = HANumber("slicer1","Slicer",ha_device,1,100,1);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    // Device configuration out of constructor
    ha_device.setName("HA-MQTT: Device with info");
    ha_device.setIdentifier("device01"); // Must be unique to avoid conflicts with other devices
    ha_device.setManufacturer("US");
    ha_device.setModel("ESP32");
    ha_device.setHwVersion("2.0.0");

    // Initialize entities
    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_number);

    ha_number.setState(30);
    HAMQTT.setCallback(ha_callback);

    // start wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED && !HAMQTT.connected()) {
         if( HAMQTT.connect("examples",MQTT_USER,MQTT_PASSWORD))
            Serial.println("Connected to MQTT");
         else
         {
            Serial.println("Failed to connect to MQTT");
            delay(5000);
         }
    }
    HAMQTT.loop();
    delay(10);
}

/* Callback from HA-MQTT entities. It is called when an entity changes its state.

    Entity can be NULL when the received topic is not related to any entity.
    This is useful to handle other topics with the same mqtt client.
*/
void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length){
    if(entity != NULL) {
        Serial.printf("Changed entity: unique_id='%s' \n",entity->getUniqueId());
    }
    else
        Serial.println("Callback called from other subscription");

}
