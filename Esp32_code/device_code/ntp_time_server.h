#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

String  getTime(){
  static bool isConfigTime = 0;
  if(isConfigTime == 0){
    Serial.println("seting time");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    isConfigTime = 1;
    
  }
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("failed to obtain time");
    //return;
  }
  char str[32];
  // strftime(str, sizeof str,"%A, %B %d %Y %H:%M:%S",&timeinfo); // Wednesday, July 26 2023 00:09:01
  strftime(str, sizeof str,"%d/%m/%y %H:%M:%S",&timeinfo);
  return str;
}


