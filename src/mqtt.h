#ifndef MQTT_H
#define MQTT_H
#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


extern void wifiConnect();
extern void wifiShutdown();
extern void mqttSendValues();
extern void mqttHandler();
extern void mqttSendWaterLevel(int level);
extern void mqttSendTemperature(float temp);
extern void mqttSendEC(float ec);
extern void mqttSendPPM(float ppmValue);
extern void mqttSendPumpState();

String composeClientID();
void mqttReconnect();
String macToStr(const uint8_t* mac);

#endif