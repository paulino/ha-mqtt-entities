/* ha-mqtt-entities library example of a binary sensor with device class set to 
 motion

- Board: ESP32*

You must set the definitions in the code under SECRETS_H

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
#define SENSOR_ID "my_motion_sensor"
#define FRIENDLY_NAME "Example of motion sensor"

HASensorBinary ha_sensor = HASensorBinary(SENSOR_ID,FRIENDLY_NAME);

void setup() {
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    ha_sensor.addFeature(HA_FEATURE_DEVICE_CLASS,"motion");
    HAMQTT.addEntity(ha_sensor);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    static unsigned long ten_second_delay = millis() + 10000;
    static bool motion_detected = false;
    HAMQTT.loop();
    if(!mqtt_client.connected() &&
            !mqtt_client.connect("examples",MQTT_USER,MQTT_PASSWORD))
        delay(1000);

    if(millis() > ten_second_delay) {
        // Change the state of the sensor every 10 seconds
        ten_second_delay = millis() + 10000;
        ha_sensor.setState(motion_detected);
        motion_detected = !motion_detected;
    }
    delay(10);

}

