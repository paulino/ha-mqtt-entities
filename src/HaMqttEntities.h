#ifndef __HAMQTT_H__
#define __HAMQTT_H__

#define HA_MQTT_VERSION_N 5
#define HA_MQTT_VERSION_S "1.0.5"

#include<haconsts.h>
#include<hadevice.h>
#include<hamqttcontroller.h>

#include<hanumber.h>
#include<haswitch.h>
#include<hatext.h>
#include<habutton.h>
#include<hasensornumeric.h>
#include<hasensortext.h>
#include<haselect.h>
#include<hasensorbinary.h>

// Singleton instance
HAMQTTController& HAMQTT = HAMQTTController::getInstance();

#endif

