#include "mqtt.h"
#include "pump.h"
#include <include/WiFiState.h>  

#ifndef RTC_USER_DATA_SLOT_WIFI_STATE
#define RTC_USER_DATA_SLOT_WIFI_STATE 33u
#endif

WiFiState state;
WiFiClient espClient;
PubSubClient client(espClient);


void wifiConnect(){


  ESP.rtcUserMemoryRead(RTC_USER_DATA_SLOT_WIFI_STATE, reinterpret_cast<uint32_t*>(&state), sizeof(state));
  unsigned long start = millis();


  if (!WiFi.resumeFromShutdown(state) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED)) {
    Serial.println("[INFO][Wifi] Cannot resume WiFi connection, connecting via begin...");
    WiFi.persistent(false);

    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(WIFI_SSID, WIFI_PASSWORD) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED)) {
      WiFi.mode(WIFI_OFF);
      Serial.println("[INFO][Wifi] Cannot connect!");
    }

  }
  Serial.println("[INFO][Wifi] Connected to Wifi");
  unsigned long duration = millis() - start;

  client.setServer(MQTT_SERVER, 1883);

  if (!client.connected()) {
     mqttReconnect();
  } 

}

void wifiShutdown(){
  client.loop();
  WiFi.shutdown(state);
  client.disconnect();
  ESP.rtcUserMemoryWrite(RTC_USER_DATA_SLOT_WIFI_STATE, reinterpret_cast<uint32_t*>(&state), sizeof(state));
  Serial.println("[INFO][Wifi] Wifi Shutdown");
  Serial.flush();
}

void mqttReconnect() {
  int reconnectTries = 10;
  while (!client.connected()) {
    if (reconnectTries == 0){
        break;
    }

    Serial.print("[INFO][MQTT] Connecting to MQTT Server: ");
    if (client.connect("hydrotower")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("[INFO][MQTT] Connection Retry in 5 seconds");
      delay(5000);
    }
    reconnectTries--;
  }
}

String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void mqttSendWaterLevel(int level){
  if(client.connected()){
    char levelStr[10];
    sprintf(levelStr, "%i", level);
    client.publish("hydroponic/tower/1/water/level", levelStr);
    delay(100);
    client.loop();
  }
}

void mqttSendTemperature(float temp){
  if(client.connected()){
    char tempStr[10];
    sprintf(tempStr, "%f", temp);
    client.publish("hydroponic/tower/1/water/temp", tempStr);
    delay(100);
  }
}

void mqttSendEC(float ec){
  if(client.connected()){
    char ecStr[10];
    sprintf(ecStr, "%f", ec);
    client.publish("hydroponic/tower/1/fertilizer/ec", ecStr);
    delay(100);
    client.loop();
  }
}

void mqttSendPPM(float ppmValue){
  if(client.connected()){
    char ppmStr[10];
    sprintf(ppmStr, "%i", (int)ppmValue);
    client.publish("hydroponic/tower/1/fertilizer/ppm", ppmStr);
    delay(100);
  }
}

void mqttSendPumpState(){
  if(client.connected()){
    if (getPumpState()){  
        client.publish("hydroponic/tower/1/pump/state", "1");
    } else {
        client.publish("hydroponic/tower/1/pump/state", "0");
    }
      delay(200);
      client.loop();
  }
}

void mqttHandler(){
    client.loop();
}

String composeClientID() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String clientId;
  clientId += "esp-";
  clientId += macToStr(mac);
  return clientId;
}
