# MQTT Home Assistant library for Arduino

Library for simplifying designs using Home Assistant (HA) MQTT-Discovery. It
includes a set of classes to create entities and publish them to HA. It also
supports joining entities to a device but is not mandatory.

It is inspired by [Arduino Home Assistant
integration](https://github.com/dawidchyrzynski/arduino-home-assistant) but with
less features.

> [!NOTE]
> This library contains only a subset of the HA-MQTT integrations. They
> are supported with minimal options. For complex designs consider the
> [Alternatives](alternatives) listed below.


## Details

The library is intended for use in ESPXX devices.
It is only tested on some ESP32

**Dependence**:

  - PubSubClient library.

**Features**:

- The library implements HA-MQTT discovery [more info] [[more
  info](https://www.home-assistant.io/integrations/mqtt/)].
- For simple designs, HA entities can be created without a device.
- For complex designs, HA entities can be grouped into a device (recommended
  solution)
- MQTT callbacks are supported in non-exclusive mode, but it is not necessary to
  use them. The state of the entities is updated when the MQTT topic is received
  and they can be polled in the main loop.
- For complex designs, the components can be extended through inheritance and
  overriding methods such as `onStateChange`.

For more information on usage, see Examples. Future documentation will be added
(work in progress).

## Components supported

- [X] Button
- [X] Text sensor
- [X] Numeric sensor
- [X] Binary sensor
- [ ] Event
- [X] Number
- [X] Switch
- [X] Text
- [X] Select

Unchecked items will be supported in future versions but complex components such
as *Alarms*,*Cameras* are not planned to be supported.

## HA MQTT properties supported (and not)

The library supports items marked:

- [X] Name
- [X] Device name
- [X] Availability
- [X] Device class
- [X] Unit of measurement
- [X] Icon
- [ ] Attributes

Unchecked items will be supported in future versions.

MQTT Device registry attributes supported:

- [X] Name
- [X] Manufacturer
- [X] Model
- [X] Software version
- [X] Hardware version
- [ ] Serial Number
- [ ] Configuration URL
- [ ] Connections
- [ ] Suggested area
- [ ] Via Device

## How to Use

A complete set of examples is in the folder `examples` but, two simple examples
are shown below.

**Example 1**: A simple entity created without a device:

```cpp
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

HASwitch ha_switch = HASwitch("hamqtt01_uid","Demo Led");

void setup() {
    pinMode(PIN_LED,OUTPUT);
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,1);
    HAMQTT.addEntity(ha_switch);

    WiFi.begin("MySSID", "MyPassword");
}

void loop() {
    bool on_off = ha_switch.getState();

    digitalWrite(PIN_LED, on_off);

    if (!mqtt_client.connected())
        mqtt_client.connect("HAMQTTExample","user","password");

    HAMQTT.loop();
}
```

**Example 2**: Creating a device and grouping entities is better:

```cpp
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

#define ENTITIES_COUNT 2
#define MAX_TEXT_LENGTH 50
HADevice ha_device("example02","Example 2 HA-MQTT","1.0");
HASwitch ha_switch = HASwitch("example02switch","Test on/off",ha_device);
HAText ha_text = HAText("example02","Input text",ha_device,MAX_TEXT_LENGTH);

void setup() {
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);

    HAMQTT.begin(mqtt_client,ENTITIES_COUNT);
    HAMQTT.addEntity(ha_switch);
    HAMQTT.addEntity(ha_text);

}
```

**Other examples**:

- Availability and icon: [availability.ino](examples/availability/availability.ino)
- Class device: [sensorbinary.ino](examples/sensorbinary/sensorbinary.ino)

## Alternatives

- [Arduino Home Assistant
integration](https://github.com/dawidchyrzynski/arduino-home-assistant)
- [HAMqttDevice](https://github.com/plapointe6/HAMqttDevice)
- [HAMqttDiscoveryHandler](https://github.com/cyijun/HAMqttDiscoveryHandler)
- [MycilaHADiscovery](https://github.com/mathieucarbou/MycilaHADiscovery)

## License

Licensed under the Apache License, Version 2.0

## Author

Designed and maintained by Paulino Ruiz de Clavijo Vázquez pruiz@us.es
