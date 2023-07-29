// #include "ADE9153A.h"
// #include "ADE9153AAPI.h"
#include "ADE9153A.h"
#include "ADE9153AAPI.h"
// #include "ADE9153AAPI.cpp"
#define ARM_MATH_CM0PLUS
#include <SPI.h>
/* Basic initializations */
#define SPI_SPEED 1000000     //SPI Speed
#define CS_PIN 5              //8-->Arduino Zero. 15-->ESP8266 
#define ADE9153A_RESET_PIN 17  //On-board Reset Pin
#define USER_INPUT 32          //On-board User Input Button Pin
#define LED 33                 //On-board LED pin
ADE9153AClass ade9153A;

void readandwrite(void);
void resetADE9153A(void);

void readandwrite(struct EnergyRegs *energyVals, 
 struct PowerRegs *powerVals,
 struct RMSRegs *rmsVals, 
 struct PQRegs *pqVals,
 struct AcalRegs *acalVals,
 struct Temperature *tempVal)
{ 
 /* Read and Print WATT Register using ADE9153A Read Library */
  ade9153A.ReadPowerRegs(powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(rmsVals);
  ade9153A.ReadPQRegs(pqVals);
  ade9153A.ReadTemperature(tempVal);
  
  Serial.print("RMS Current:\t");        
  Serial.print(rmsVals->CurrentRMSValue/1000); 
  Serial.println(" A");
  
  Serial.print("RMS Voltage:\t");        
  Serial.print(rmsVals->VoltageRMSValue/1000);
  Serial.println(" V");
  
  Serial.print("Active Power:\t");        
  Serial.print(powerVals->ActivePowerValue/1000);
  Serial.println(" W");
  
  Serial.print("Reactive Power:\t");        
  Serial.print(powerVals->FundReactivePowerValue/1000);
  Serial.println(" VAR");
  
  Serial.print("Apparent Power:\t");        
  Serial.print(powerVals->ApparentPowerValue/1000);
  Serial.println(" VA");
  
  Serial.print("Power Factor:\t");        
  Serial.println(pqVals->PowerFactorValue);
  
  Serial.print("Frequency:\t");        
  Serial.print(pqVals->FrequencyValue);
  Serial.println(" Hz");
  
  Serial.print("Temperature:\t");        
  Serial.print(tempVal->TemperatureVal);
  Serial.println(" degC");
  // lcdprint(String(rmsVals.VoltageRMSValue/1000) + "V " + String(rmsVals.CurrentRMSValue/1000) + "A " + String(pqVals.FrequencyValue)+ "HZ", String(tempVal.TemperatureVal) + "degC" + getTime() );
  Serial.println("");
  Serial.println("");
}

void resetADE9153A(void)
{
 digitalWrite(ADE9153A_RESET_PIN, LOW);
 vTaskDelay(1000 / portTICK_PERIOD_MS);
 digitalWrite(ADE9153A_RESET_PIN, HIGH);
 delay(1000);
 vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void setupADE9153A(){
  resetADE9153A();            //Reset ADE9153A for clean startup
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  /*SPI initialization and test*/
  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
  if (!commscheck) {
    // line1 = "Metering error";
    // line2 = "Contact to admin";
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while (!commscheck) {   
      commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI  //Hold until arduino is reset
      Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
      // vTaskDelay(1000 / portTICK_PERIOD_MS);
      delay(100);
    }
  }
  
  ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
  /* Read and Print Specific Register using ADE9153A SPI Library */
  Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
  ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
  vTaskDelay(500 / portTICK_PERIOD_MS);
}