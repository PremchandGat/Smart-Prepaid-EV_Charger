#include <HTTPClient.h>
String GOOGLE_SCRIPT_ID = "AKfycbz6n04QLs-P0iU4qlGYthZ6PsZ2nDh64-4Fpe2dpxSFzlR-6RhEcedTUfxy5yWskYQTDQ";    // change Gscript I

String sendDataToSheet(String time, String rfid, float* current, float* voltage, float* reactive_power, float* apparent_power, float* power_factor, float* frequiency, float* tempareture, float* total_energy, float *active_power){
    time.replace(" ", "-");
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"date=" + time + "&rfid=" + rfid+ "&current=" + String(*current, 3)+"&voltage=" + String(*voltage, 3)+"&active_power=" + String(*active_power, 3)+"&reactive_power=" + String(*reactive_power,3)+"&apparent_power=" + String(*apparent_power, 3)+"&frequiency=" + String(*frequiency,3)+"&tempareture=" + String(*tempareture,2)+"&power_factor=" + String(*power_factor,3)+"&total_energy=" + String(*total_energy, 3);
    HTTPClient http;
 //   urlFinal = "https://script.google.com/macros/s/AKfycbz6n04QLs-P0iU4qlGYthZ6PsZ2nDh64-4Fpe2dpxSFzlR-6RhEcedTUfxy5yWskYQTDQ/exec?date=&rfid=&current=0.000&voltage=0.000&active_power=0.000&reactive_power=0.000&apparent_power=0.000&frequiency=262144000.000&tempareture=0.00&power_factor=0.000&total_energy=17.110";
   // HttpClient http;
    Serial.print(urlFinal);
    http.begin(urlFinal.c_str());
     http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: "); 
    Serial.println(httpCode);
    String payload = "";
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    http.end();
    return payload;
  }
  
