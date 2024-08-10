/* ha-mqtt-entities library example: later device configuration and multiple
   devices

- Board: ESP32*

This example creates multiple devices and configure the device ID and friendly
name in the setup function

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
HADevice ha_device_1 = HADevice(SW_VERSION);
HANumber ha_number_1 = HANumber("numberexample","Slicer",ha_device_1,1,100,1);
HAText ha_text_1 = HAText("text","Input text",ha_device_1,100);

HADevice ha_device_2 = HADevice(SW_VERSION);
HANumber ha_number_2 = HANumber("numberexample","Slicer",ha_device_2,1,100,1);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    // Delayed device configuration
    ha_device_1.setName("Example: Device 1");
    ha_device_1.setIdentifier("device01");
    ha_device_2.setName("Example: Device 2");
    ha_device_2.setIdentifier("device02");

    // Initialize entities
    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_number_1);
    HAMQTT.addEntity(ha_number_2);
    HAMQTT.addEntity(ha_text_1);

    ha_number_1.setState(30);
    ha_number_2.setState(90);
    ha_text_1.setState("Hello HomeAssistant");
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
    if(entity != NULL) {
        Serial.printf("Changed entity: unique_id='%s' \n",entity->getUniqueId());
    }
    else
        Serial.println("Callback called from other subscription");

}
