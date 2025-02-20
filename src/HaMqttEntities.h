#ifndef __HAMQTT_H__
#define __HAMQTT_H__

#define HA_MQTT_VERSION_N 10
#define HA_MQTT_VERSION_S "1.0.10"

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

// Singleton instance via extern for Arduino library
extern HAMQTTController& HAMQTT;

#endif

