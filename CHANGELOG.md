# Changelog

## [1.0.10] - 2025-02-21

- Fixed bug for entities not grouped in devices
- Improved *lastwill* device configuration
- Removed extra delay in some examples

## [1.0.9] - 2024-12-10

- Fixed bug in HADevice constructor
- Added some features as `progmem` strings to save memory

## [1.0.8] - 2024-10-31

- Fixed bug with some features in entities
- Fixed bug in availability for devices

## [1.0.7] - 2024-10-22

 - Fixed Bug on MQTT reconnection. Device availability was not sent to HA

## [1.0.6] - 2024-09-22

 - Fixed bug in reconnection to HA when MQTT connection is lost

## [1.0.5] - 2024-08-28

 - Fixed bug in reconnection loop
 - Added support for LWT
 - Added availability for entities and/or devices
 - Added device attributes: hw_version, manufacturer, model
 - Added Binary Sensor
 - Added configuration features: device_class and icon
 - Optimized discovery payload to use abbreviations

## [1.0.4] - 2024-08-10

 - Fixed bug in unique_id when multiple devices are created
 - Improved device constructor for later setup identifier and friendly name
 - Added example for multiple devices and devices configured in the setup
   function

## [1.0.3] - 2024-04-17

- Fixed MQTT reconnection bug
- Fixed resend all states every 24h
- Updated all examples to use the new connect method

## [1.0.2] - 2024-03-18

- Added *unit_of_measurement* configuration to HASensorNumeric
- Added new example for HASensorNumeric to get a graph history in HomeAssistant

## [1.0.1] - 2024-03-17

- Fixed bug in HANumber configuration payload
