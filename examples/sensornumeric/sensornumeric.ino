/* ha-mqtt-entities library example of a sensor that shows
    history graph in Home Assistant.

- Board: ESP32*

You must set the definitions in the code under SECRETS_H.

*/
#include<Arduino.h>
#include<WiFi.h>
#include<PubSubClient.h>
#include<mat.h>

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
#define HA_DEVICE_ID   "example04"
#define HA_DEVICE_FRIENDLY_NAME "Example Numeric-Sensor HA-MQTT"

#define UNIT_OF_MEASUREMENT "V"
#define PRECISION 2  // Number of decimals

HADevice ha_device = HADevice(HA_DEVICE_ID,HA_DEVICE_FRIENDLY_NAME,"1.0");

HASensorNumeric ha_sensor = HASensorNumeric(
    "sensor05uid","Volts",ha_device,UNIT_OF_MEASUREMENT,PRECISION);

void setup() {
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_sensor);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    static unsigned long one_second = -1000;
    static float counter = 0;
    HAMQTT.loop();
    if(!mqtt_client.connected())
         mqtt_client.connect("examples",MQTT_USER,MQTT_PASSWORD);

    if(millis() - one_second > 1000){
        // Update the sensor every second creating a sin wave
        one_second = millis();
        counter+=0.1;
        ha_sensor.setState(3.3*sin(counter));
    }

}

