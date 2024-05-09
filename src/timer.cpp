#include "timer.h"
#include "pump.h"
#include "mqtt.h"
#include "sensors.h"
#include "config.h"

noDelay hydroTimer(PUMP_INTERVAL_IN_SECONDS * 1000, timerCallback);

void timerHandler(){
      hydroTimer.update();
}

void timerCallback(){
  Serial.println("[INFO][Timer]: Reading Values.");

  wifiConnect();
  
  mqttSendEC(getTDSEC());
  mqttSendPPM(getTDSPPM());
  mqttSendTemperature(getTemperature());
  mqttSendWaterLevel(getWaterLevelPercent());

  pumpOff();
  mqttSendPumpState();

  wifiShutdown();

  Serial.println("[INFO][Timer]: Going to sleep.");
  ESP.deepSleep(TIME_DEEP_SLEEP_IN_SECONDS * 1000000L);

}



