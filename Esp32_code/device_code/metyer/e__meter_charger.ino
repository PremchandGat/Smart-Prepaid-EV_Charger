#define ARM_MATH_CM0PLUS
#include <SPI.h> 
#include <ADE9153A.h>
#include <ADE9153AAPI.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
//defining the statement, basic initiating
#define SPI_SPEED 1000000 //SPI Speed
#define CS_PIN 5 
#define ADE9153A_RESET_PIN 17 //On-board Reset Pin
#define USER_INPUT 33 //On-board User Input Button Pin
#define LED 2 //On-board LED pin
#define Relay_Pin 17 //Relay pin
#define SS_PIN D2
#define RST_PIN D1
LiquidCrystal_I2C lcd(0x27, 16, 2);
ADE9153AClass ade9153A;


//ADE9153A
struct EnergyRegs energyVals; //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals; //Metrology data can be accessed from these structures
struct RMSRegs rmsVals; //RMS Value data store in new registor
struct PQRegs pqVals;//power factor resistor value 
struct AcalRegs acalVals; //total value store in registor 
struct Temperature tempVal;//tem. variabe 
//Functions ADE9153A
void readandwrite(void);
void resetADE9153A(void);

int ledState = LOW;
int inputState = LOW;
 unsigned long currentReport = millis();
unsigned long lastReport = 0;
const  long reportInterval = 2000;
const long blinkInterval = 500;
  float sumkillowatt = 0.0;
  float newsumkillowatt = 0.0;
int sec_last;

void readandwrite()
{
/* Read and Print WATT Register using ADE9153A Read Library */
 ade9153A.ReadPowerRegs(&powerVals); //Template to read Power registers from ADE9000 and 
 ade9153A.ReadRMSRegs(&rmsVals);
 ade9153A.ReadPQRegs(&pqVals);
 ade9153A.ReadTemperature(&tempVal);
 
 Serial.print("RMS Current:\t"); 
 Serial.print(rmsVals.CurrentRMSValue/1000);
 Serial.println(" A");

 Serial.print("RMS Voltage:\t"); 
 Serial.print(rmsVals.VoltageRMSValue/1000);
 Serial.println(" V");
 
 Serial.print("Active Power:\t"); 
 Serial.print(powerVals.ActivePowerValue/1000);
 Serial.println(" W");
 sumkillowatt = powerVals.ActivePowerValue/1000;
 newsumkillowatt = newsumkillowatt + sumkillowatt;
 
 Serial.print("Reactive Power:\t"); 
 Serial.print(powerVals.FundReactivePowerValue/1000);
 Serial.println(" VAR");

 Serial.print("Apparent Power:\t"); 
 Serial.print(powerVals.ApparentPowerValue/1000);
 Serial.println(" VA");

 Serial.print("Power Factor:\t"); 
 Serial.println(pqVals.PowerFactorValue);
 
 Serial.print("Frequency:\t"); 
 Serial.print(pqVals.FrequencyValue);
 Serial.println(" Hz");

 Serial.print("Temperature:\t"); 
 Serial.print(tempVal.TemperatureVal);
 Serial.println(" degC");
 Serial.println("");
 Serial.println("");
}
void resetADE9153A(void)
{
digitalWrite(ADE9153A_RESET_PIN, LOW);
delay(100);
digitalWrite(ADE9153A_RESET_PIN, HIGH);
delay(1000);
Serial.println("Reset Done");
}

void setup() {
   Serial.begin(115200);
   resetADE9153A();
   delay(500);
   bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
 if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the ESP8266/ESP32");
  while (!commscheck) { //Hold until arduino is reset
    delay(1000);
 }
 }
 ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
// /* Read and Print Specific Register using ADE9153A SPI Library */
 Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
//
 ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
 delay(500);

    pinMode(USER_INPUT, INPUT); //user botton press then take input 
    pinMode(ADE9153A_RESET_PIN, OUTPUT); //ade9153a reset pin take output
    digitalWrite(ADE9153A_RESET_PIN, HIGH); //ade9153 reset pin high 
      pinMode(Relay_Pin, OUTPUT); //relay pin ready for output
 digitalWrite(Relay_Pin, LOW); //pin 8 should be low initially to start sensing operation. 
 delay(2000);
 }

void loop(){
  // put your main code here, to run repeatedly:
readandwrite();
 while (rmsVals.CurrentRMSValue/1000 >= 2.0)
 {
 digitalWrite(Relay_Pin, HIGH); // turn relay on
 Serial.print("Relay Activated");
 lcd.setCursor(3,0);
 lcd.print("OVER_CURRENT");
 delay(1000);
  lcd.clear();
  lcd.setCursor(3,1);
 lcd.print("REMOVE_LOAD");
 delay(1000);
  lcd.clear();
 }
 if (rmsVals.VoltageRMSValue/1000 >= 250.0 )
 {
 digitalWrite(Relay_Pin, HIGH); // turn relay on
 Serial.print("Relay Activated");
 lcd.print("Relay Activated");
 Serial.println("it is working");
 lcd.print("OVER_VOLTAGE");
 delay(1000);
  lcd.clear();
 }
 //if ((rmsVals.VoltageRMSValue/1000 <= 250.0) && (tempVal.TemperatureVal <= 55.0))
 //{ 
 //digitalWrite(Relay_Pin, LOW);
 //} // turn relay off
 if(tempVal.TemperatureVal >= 60.0)
 {
 digitalWrite(Relay_Pin, HIGH); // turn relay on
 Serial.print("Relay Activated");
 }

 lcd.clear();
 lcd.setCursor(9,0);
 lcd.print("T:");
 lcd.print(tempVal.TemperatureVal);
 lcd.setCursor(14,1);
 lcd.print((char)223); // degree symbol 
 lcd.println("C");
 lcd.setCursor(0,0); // Start at top-left corne
 lcd.print("I:"); 
 lcd.print(rmsVals.CurrentRMSValue/1000);
 lcd.print(" A");
 lcd.setCursor(0,1);
 lcd.print("V:"); 
 lcd.print(rmsVals.VoltageRMSValue/1000);
 lcd.print(" V");
 if ((currentReport - lastReport) >= reportInterval){
 lastReport = currentReport;
// Sheetsprint();
}
 inputState = digitalRead(USER_INPUT);
 if (inputState == LOW) {
  Serial.println("Autocalibrating Current Channel");
  ade9153A.StartAcal_AINormal();
//  runLength(20);
  ade9153A.StopAcal();
  Serial.println("Autocalibrating Voltage Channel");
  ade9153A.StartAcal_AV();
//  runLength(40);
  ade9153A.StopAcal();
  delay(100);
 
  ade9153A.ReadAcalRegs(&acalVals);
  Serial.print("AICC: ");
  Serial.println(acalVals.AICC);
  Serial.print("AICERT: ");
  Serial.println(acalVals.AcalAICERTReg);
  Serial.print("AVCC: ");
  Serial.println(acalVals.AVCC);
  Serial.print("AVCERT: ");
  Serial.println(acalVals.AcalAVCERTReg);
  long Igain = (-(acalVals.AICC / 838.190) - 1) * 134217728;
  long Vgain = ((acalVals.AVCC / 13411.05) - 1) * 134217728;
  ade9153A.SPI_Write_32(REG_AIGAIN, Igain);
  ade9153A.SPI_Write_32(REG_AVGAIN, Vgain);
 
  Serial.println("Autocalibration Complete");
  delay(2000);
 }
}
