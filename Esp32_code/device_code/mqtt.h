#include "HardwareSerial.h"
#include "Arduino.h"
#include <PubSubClient.h>
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#include "ArduinoJson.h"
// #include "vars.h"
//const char *mqtt_server = "192.168.10.204";
const char *mqtt_server = "192.168.60.208";
const char *deviceID = "station1";
const char *publishTopic = "ECB/EIC/station1/out"; 
const char *subscribeTopic = "ECB/EIC/station1/in";
// const char *server = "192.168.43.171:4004";
boolean isMqttSetupDone = false;
// enum requestStatus {turnOn, turnOff,  } reqStatus;
int reqStatus = 0 ; // 0 = unknown , 1 = connected, 2= disconnect
enum mqttStatus {mqttOffline, mqttLive, mqttConnecting } statusOfMqtt;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void pulishData(float volt, float current, float power, float energy, float temp){
 if (client.connected())
    {
      String data = "{\"v\":\"" + String(volt) + "\",\"c\":\"" + String(current) + "\",\"p\":\"" + String(power) + "\",\"e\":\"" + String(energy)  + "\",\"power\":\"" + String(power) + "\"}";
      client.publish(publishTopic, data.c_str());
    }
}

char* getMqttStatusMessage(){
  if (statusOfMqtt == mqttConnecting) {
    return "Connecting";
  }
  else if (statusOfMqtt == mqttLive) {
    return "LIVE";
  }
  return "Offline";
} 

// callback for MQTT PubSubClient
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  // use the Stream interface to print the contents
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  String card = doc["card"];

  if(card.length() > 3){
    reqStatus = 1;
  }

  if(doc["status"] == "off" ){
  reqStatus = 2;
  }

  // String lmt = doc["limit"];
  // String bal = doc["card_balance"];
  // String year = doc["year"];
  // String month = doc["month"];
  // String day = doc["date"];
  // String hour = doc["hour"];
  // String minute = doc["minutes"];
  // String seconds = doc["seconds"];
}

// This function check weather device is connected to MQTT broker or not if not then it will connect to mqtt
void connectToMQTT(void *updateStatus)
{
  // TaskFunction_t functionPtr =
  // TaskFunction_t functionPtr = reinterpret_cast<TaskFunction_t>(updateStatus);
  void (*updateStatusFunc)(int) = (void (*)(int))updateStatus;

  (void)updateStatusFunc(1);
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
      (void)updateStatusFunc(1);
      if (WiFi.status() == WL_CONNECTED)
      {
        if (!client.connected())
        {
          statusOfMqtt = mqttConnecting;
           if(!isMqttSetupDone) {
                  client.setServer(mqtt_server, mqtt_port);
                  client.setCallback(callback);
                  isMqttSetupDone = true;
            }
          Serial.print("Attempting MQTT connection...");
          // Create a random client ID
          String clientId = "flutter";
          client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD);
          Serial.println("connected");
          client.publish(publishTopic, clientId.c_str());
          client.subscribe(subscribeTopic);
          vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
        else
        {
          statusOfMqtt = mqttLive;
          Serial.println("MQTT already connected");
          // int x = (int)status;
          // x = reqStatus; 
       
          // Serial.println(reqStatus);
          // Serial.println(*status);
          if(reqStatus != 0){
                    }
          reqStatus = 0;
          // Serial.println(x);
          vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        // get messages from server
        client.loop();
      }
      else
      {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by MQTT handler is : ");
    Serial.println(uxHighWaterMark);
  }
}
