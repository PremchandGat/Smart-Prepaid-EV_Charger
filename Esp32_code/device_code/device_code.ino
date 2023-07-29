#include "lcd_print.h"
#include "wifiSetup.h"
#include "ntp_time_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "http.h"
#include "mqtt.h"
#include "metering.h"
#include "vars.h"

// extern String line1 = "WELCOME EI&CE   ";
// extern String line2 = "PREMCHAND       ";

struct EnergyRegs energyVals;  //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  
struct PQRegs pqVals;
struct AcalRegs acalVals;
struct Temperature tempVals;
// enum userStatus {connect, disconnect , unknown} status;
int status = 0; // 0 = unknown , 1 = connected, 2= disconnect


int ledState = LOW;
int inputState = LOW;
unsigned long lastReport = 0;
const long reportInterval = 2000;
const long blinkInterval = 500;

TaskHandle_t connectToWiFiHandler = NULL;
TaskHandle_t connectToMQTTHandler = NULL;
TaskHandle_t energyMeateringHandler = NULL;
TaskHandle_t chargingTaskHandler = NULL;
TaskHandle_t lcdHandler = NULL;

void turnOnAndOffRelay(boolean x){
  if(x){
    digitalWrite(relay, HIGH);
  }else{
    digitalWrite(relay, LOW);
  }
  status = 0;
}

void updateStatus(int st){
  Serial.print("Update Status to");
  Serial.println(st);
  status = st;
}

void chargingTask(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
    if(status == 0){
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    else if(status == 1) {
      turnOnAndOffRelay(true);
      vTaskDelay(1000/ portTICK_PERIOD_MS );
    }
    else if(status == 2){
      turnOnAndOffRelay(false);
      vTaskDelay(1000/ portTICK_PERIOD_MS );
    }
  }
}


void meteringTask(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
  readandwrite(&energyVals,    &powerVals,     &rmsVals,   &pqVals,  &acalVals,  &tempVals);
  line1 = String(rmsVals.VoltageRMSValue/1000) + "V " + String(rmsVals.CurrentRMSValue/1000) + "A " + String(pqVals.FrequencyValue)+ "HZ" ;
  line2 = String(tempVals.TemperatureVal) + "˚C";
  pulishData(rmsVals.VoltageRMSValue/1000, rmsVals.CurrentRMSValue/1000, tempVals.TemperatureVal, pqVals.FrequencyValue, tempVals.TemperatureVal);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
  Serial.print("maxdata used by wifi connection is : ");
  Serial.println(uxHighWaterMark);
  }
}


void lcdPrintTask(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
    line3 = getTime();
    line4 = String(getWifiStatusMessage()) + String(getMqttStatusMessage());
    printOnLcd(line1,  line2, line3, line4);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // clears the display to print new message
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by wifi connection is : ");
    Serial.println(uxHighWaterMark);
  }
}

void setup() {
  /* Pin and serial monitor setup */
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  turnOnAndOffRelay(false);
  xTaskCreatePinnedToCore(
        lcdPrintTask,         // function name which you want to run
        "Handle LCD",     // function description
        10000,                  // stack size
        NULL,                  // parameters of function
        1,                     // task priority
        &lcdHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );
    xTaskCreatePinnedToCore(
        chargingTask,         // function name which you want to run
        "Handle LCD",     // function description
        10000,                  // stack size
        NULL,                  // parameters of function
        1,                     // task priority
        &chargingTaskHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );
  Serial.print("Started");
  delay(1000);
  pinMode(LED, OUTPUT);
  pinMode(USER_INPUT, INPUT);
  setupADE9153A();
  
  xTaskCreatePinnedToCore(
        connectToWiFi,         // function name which you want to run
        "Connect to wifi",     // function description
        10000,                  // stack size
        NULL,                  // parameters of function
        1,                     // task priority
        &connectToWiFiHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );

    xTaskCreatePinnedToCore(
        meteringTask,         // function name which you want to run
        "Handle Metering",     // function description
        10000,                  // stack size
        NULL,                  // parameters of function
        1,                     // task priority
        &energyMeateringHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );

    xTaskCreatePinnedToCore(
        connectToMQTT,         // function name which you want to run
        "Handle MQTT",     // function description
        10000,                  // stack size
        (void*)&updateStatus,                  // parameters of function
        1,                     // task priority
        &connectToMQTTHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );

}

void loop() {
 vTaskDelay(5000 / portTICK_PERIOD_MS);
}
