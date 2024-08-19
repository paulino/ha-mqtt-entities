#ifndef __HAMQTT_H__
#define __HAMQTT_H__

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

