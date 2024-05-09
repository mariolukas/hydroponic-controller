#include "pump.h"

bool pumpState = false;

void pumpInit(){
  Serial.println("[INFO][Pump]: Initialized");    
  pinMode(PUMP_PIN, OUTPUT);

}

void pumpOn(){
  Serial.println("[INFO][Pump]: On");  
  //analogWrite(PUMP_PIN, 255);
  digitalWrite(PUMP_PIN, LOW);
  pumpState = true;
}

void pumpOff(){
  Serial.println("[INFO][Pump]: Off");  
  digitalWrite(PUMP_PIN, HIGH);
  pumpState = false;
}

bool getPumpState(){
  return pumpState;
}