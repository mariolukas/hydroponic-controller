#ifndef SENSORS_H
#define SENSORS_H
#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ultrasonic.h>
#include <CQRobotTDS.h>

extern void handleSensors();
extern int getWaterLevel();
extern int getWaterLevelPercent();
extern float getTemperature();
extern float getTDSPPM();
extern float getTDSEC();

#endif