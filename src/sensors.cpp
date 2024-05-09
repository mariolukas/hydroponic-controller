#include "sensors.h"

float tdsppm= 0;
float tdsec = 0;
float temp = 20.0;

OneWire oneWire( TEMP_WIRE_BUS );
DallasTemperature sensors( &oneWire );
CQRobotTDS tds(TDS_PIN);
Ultrasonic ultrasonic(SR04_TRIGGER_PIN,SR04_ECHO_PIN);

int getWaterLevel(){
  int level = ultrasonic.Ranging(CM); 
  Serial.print("[INFO][Sensor Water Level]: ");
  Serial.print(level); // CM or INC
  Serial.println(" cm" );
  return level;
}

int getWaterLevelPercent(){
  int level = ultrasonic.Ranging(CM); 
  if(level > MIN_WATER ) level = MIN_WATER;
  if(level < MAX_WATER ) level = MAX_WATER;
  level = map(level, MIN_WATER, MAX_WATER, 0, 100);
  Serial.print("[INFO][Sensor Water Level]: ");
  Serial.print(level); // CM or INC
  Serial.println(" %" );
  return level;
}

float getTDSPPM(){
  Serial.print("[INFO][Sensor TDS PPM]: ");
  Serial.print(tdsppm); // CM or INC
  Serial.println(" ppm" );
  return tdsppm;
}

float getTDSEC(){
  Serial.print("[INFO][Sensor TDS EC]: ");
  Serial.print(tdsec); // CM or INC
  Serial.println(" ms/cm" );
  return tdsec;
}

float getTemperature(){
  Serial.print("[INFO][Sensor Water Temperature]: ");
  Serial.print(temp); // CM or INC
  Serial.println(" °C" );
  return temp;
}

void handleSensors(){
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  tdsppm = tds.update(temp);
  tdsec = (tdsppm * 2/1000);
}