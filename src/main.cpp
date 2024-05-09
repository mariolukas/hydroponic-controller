#include "timer.h"
#include "sensors.h"
#include "pump.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  
  wifiConnect();
  pumpInit();
  pumpOn();
  mqttSendPumpState();
  wifiShutdown();

}

void loop() {
  timerHandler();
  handleSensors();
}

