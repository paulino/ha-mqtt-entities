/* ha-mqtt-entities library simple example

- Board: ESP32*

This example creates a MQTT switch entity in the Home Assistant to toggle the
onboard LED.

In this example, no MQTT device is created in HA, only a switch entity is added.
No callback is added either, the switch state is polled in the main loop.

You must set the definitions in the code under SECRETS_H.

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

#define PIN_LED 2  // On board LED in ESP32 DevKit v1
#define ENTITIES_COUNT 1
#define SWITCH_UID "example01switch"  // Be careful of be unique in HA entities list

HASwitch ha_switch = HASwitch(SWITCH_UID,"Demo Led");

void setup() {
    pinMode(PIN_LED,OUTPUT);

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_switch);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    bool on_off = ha_switch.getState();
    digitalWrite(PIN_LED, on_off);
    if (!mqtt_client.connected())
        mqtt_client.connect("HAMQTTExample",MQTT_USER,MQTT_PASSWORD);
    HAMQTT.loop();
}
