#include "WiFi.h"
// char* ssid = "ECB_TEQIP"; //your wifi ssid or name
// char* password = ""; //your wifi password 
char* ssid = "Galaxy A12 A78B"; //your wifi ssid or name
char* password = "sgxq3456"; //your wifi password

// Status of Wifi Status
enum WiFiStatus{ WifiUnknown, WifiConnecting, WifiConnected, WifiDisconnected } statusOfWifi;

char* getWifiStatusMessage(){
 if (statusOfWifi == WifiConnecting) {
    return "Wifi Connecting... ";
  } else if (statusOfWifi == WifiConnected) {
    return "WiFi Connected ";
  }
  return "Wifi Unknown ";
}

void connectToWiFi(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      statusOfWifi = WifiConnected;
      Serial.println("wifi connected already");
      Serial.print("Wifi ip address is : ");
      Serial.println(WiFi.localIP());
      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    else
    {
      statusOfWifi = WifiConnecting;
      Serial.println("Connecting to wifi");
      WiFi.begin(ssid, password);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by wifi connection is : ");
    Serial.println(uxHighWaterMark);
  }
}