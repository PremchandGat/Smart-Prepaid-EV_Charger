// Select your modem:
#define TINY_GSM_MODEM_SIM800 // Modem is SIM800L
#define SerialAT Serial1
#define GSM_PIN ""
// Your GPRS credentials, if any
const char apn[] = "airtelgprs.com";
const char gprsUser[] = "";
const char gprsPass[] = "";
// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

// include libraries which are needed
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <MFRC522.h>
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <TinyGsmClient.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <ArduinoHttpClient.h>

#define PZEM_RX_PIN 16 // PZEM module rx pin
#define PZEM_TX_PIN 17 // PZEM module tx pin
#define PZEM_SERIAL Serial2
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN); // set the PZEM RX and TX pin for serial communication
LiquidCrystal_I2C lcd(0x27, 20, 4);                      // set the LCD address to 0x3F for a 16 chars and 2 line display
RTC_DS1307 rtc;

TinyGsm modem(SerialAT);

TinyGsmClient gsmClient(modem);
PubSubClient mqtt(gsmClient);

// TTGO T-Call pins
#define MODEM_TX 32
#define MODEM_RX 35
uint32_t lastReconnectAttempt = 0;

// led indecator vare
const int redPin = 33;   // 13 corresponds to GPIO13
const int greenPin = 25; // 12 corresponds to GPIO12
const int bluePin = 26;  // 14 corresponds to GPIO14
const int freq = 5000;

const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

// led indicator colors
int red = 255;
int green = 255;
int blue = 255;

#define mqtt_port 1883
#define MQTT_USER "ESP"

#define MQTT_PASSWORD "3EjMIy89qzVn"
const char *mqtt_server = "3.111.88.241";
const char *server = "192.168.43.171:4004";
String line1 = "Starting charger";
String line2 = "Please wait...";
String line3 = "Anemoi Technologies";
String line4 = "Welcome!";
const char *ssid = "Raju";
const char *password = "Raju@123";
String busyWith = "null";
int limit = 0;
const char *deviceID = "/Ev_Charger/Premchand";
SPIClass spiSD(HSPI);
#define SD_CS 15
const int relay = 27; // Relay driver used on and off charging
boolean useGSM = true;

// Constants pins for RFID module
#define SS_PIN 5
#define RST_PIN 0
// Parameters
const int ipaddress[4] = {103, 97, 67, 25};
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Task handler
TaskHandle_t connectToWiFiHandler = NULL;
TaskHandle_t connectToMQTTHandler = NULL;
TaskHandle_t readRfidCardHandler = NULL;
TaskHandle_t energyMeateringHandler = NULL;

// data structure for logs in sd card
struct logData
{
  String time;
  String userID;
  float energy;
  float power;
  float current;
  float voltage;
  String transactionID;
  String error;
  String isPaymentDone;
  int limitTocharge;
};

// data structure for reading parameters
struct physicalPerameters
{
  String energy = "--";
  String power = "--";
  String current = "--";
  String voltage = "--";
};

// read perameters from pzem
struct physicalPerameters
readPzem()
{
  struct physicalPerameters data;
  data.voltage = String(pzem.voltage());
  data.current = String(pzem.current());
  data.power = String(pzem.power());
  data.energy = String(pzem.energy());
  // Check if the data is valid
  if (data.voltage == "nan")
  {
    data.voltage = "--";
    data.power = "--";
    data.current = "--";
    data.energy = "--";
    blue = 0;
    green = 0;
    red = 255;
    Serial.println("Error reading voltage");
  }
  else if (data.current == "nan")
  {
    data.current = "--";
    Serial.println("Error reading current");
  }
  else if (data.power == "nan")
  {
    data.power = "--";
    Serial.println("Error reading power");
  }
  else if (data.energy == "nan")
  {
    data.energy = "--";
    Serial.println("Error reading energy");
  }
  else
  {
    // Print the values to the Serial console
    Serial.print("Voltage: ");
    Serial.print(data.voltage);
    Serial.println("V");
    Serial.print("Current: ");
    Serial.print(data.current);
    Serial.println("A");
    Serial.print("Power: ");
    Serial.print(data.power);
    Serial.println("W");
    Serial.print("Energy: ");
    Serial.print(data.energy);
    Serial.println("kWh");
  }
  return data;
}

// verify user account and start charging
void verifyUserAndStartCharging(String userId, int lmt)
{
  int httpResponseCode = 0;
  String payload = "";
 if(modem.isGprsConnected()){
    Serial.print("HIT server with GSM");
    HttpClient http(gsmClient, "3.111.88.241", 4004);
    String contentType = "application/json";
    String dataa = "{\"card_no\":\"" + String("8384909995") + "\"}";
    http.post("/verifycard", contentType, dataa);
    httpResponseCode = http.responseStatusCode();
    Serial.print("Response status code: ");
    Serial.print(httpResponseCode);
    payload = http.responseBody();
    Serial.println("Response:");
    Serial.print(payload);
  } else if(WiFi.status() == WL_CONNECTED){
    Serial.print("HIT server with WIFI");
    HTTPClient http;
    http.begin("https://7o6wmrpig1.execute-api.ap-south-1.amazonaws.com/production/verifycard"); // Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");                                          // Specify content-type header
    httpResponseCode = http.POST("{\"card_no\":\"" + String(userId) + "\"}");                // Send the actual POST request
    Serial.println("Verifying card and response code is");
    Serial.println(httpResponseCode);
    payload = http.getString();
   }
  if (httpResponseCode > 0)
  {
    if (httpResponseCode == 200)
    {
      Serial.println(payload);
      StaticJsonDocument<384> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      int bal = int(doc["current_balance"]);
      busyWith = userId;
      
      if (bal > lmt)
      {
        limit = lmt;
      }
      else
      {
        limit = bal;
      }
      pzem.resetEnergy();
      struct physicalPerameters readings = readPzem();
      logSDCard(getTimeNow(), busyWith, readings.energy.toFloat(), readings.voltage.toFloat(), readings.power.toFloat(), readings.current.toFloat(), "Error name", generateTransactionID(busyWith), limit, "NO");
      digitalWrite(relay, HIGH);
    }
    else
    {
      line1 = "Card is not valid";
      if (client.connected())
      {
        String data = "{\"message\":\"card not valid\"}";
        client.publish(deviceID, data.c_str());
      }
      return;
    }
  }
  else
  {
    return;
  }
}

// get current time
String getTimeNow()
{
  DateTime now = rtc.now();
  return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + "||" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

// generate transaction ID
String generateTransactionID(String card)
{
  String id = getTimeNow() + String(random(999999999)) + String(deviceID) + card;
  //  Serial.println("Generated transaction id is -------------------------------------------------------------------------------------------");
  //  Serial.println(id);
  return id;
}

// deduct money from user card this function won't complete until money would be deducted from user account
void deductMoney(float unit, String card_no, String transactionID)
{
  blue = 0;
  green = 255;
  red = 255;
  line1 = "Processing Payment";
  line2 = "Please wait...";
  int httpResponseCode = 0;
  if(modem.isGprsConnected()){
    Serial.print("");
    HttpClient http(gsmClient, "3.111.88.241", 4004);
    String contentType = "application/json";
    String dataa = "{\"unit\":\"" + String(unit) + "\",\"card_no\":\"" + String(card_no) + "\",\"transactionID\":\"" + transactionID + "\"}";
    http.post("/deduct", contentType, dataa);
    httpResponseCode = http.responseStatusCode();
   }
  else if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://7o6wmrpig1.execute-api.ap-south-1.amazonaws.com/production/deduct");                                                                     // Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");                                                                                                          // Specify content-type header
    httpResponseCode = http.POST("{\"unit\":\"" + String(unit) + "\",\"card_no\":\"" + String(card_no) + "\",\"transactionID\":\"" + transactionID + "\"}"); // Send the actual POST request
    http.end();
   }
if (httpResponseCode == 200)
    {
      blue = 0;
      green = 255;
      red = 0;
      struct physicalPerameters readings = readPzem();
      logSDCard(getTimeNow(), card_no, unit, readings.voltage.toFloat(), readings.power.toFloat(), readings.current.toFloat(), "Error name", transactionID, limit, "Yes");
      return;
    }
    else
    {
      blue = 0;
      green = 0;
      red = 255;
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      struct physicalPerameters readings = readPzem();
      logSDCard(getTimeNow(), card_no, unit, readings.voltage.toFloat(), readings.power.toFloat(), readings.current.toFloat(), "Not able to deduct amount response code: " + String(httpResponseCode), transactionID, limit, "NO");
      deductMoney(unit, card_no, transactionID);
    }
}

// Sd card modules functions start

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char *message)
{
  Serial.printf("Appending to file: /chargerLogs.txt\n");

  File file = fs.open("/chargerLogs.txt", FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

// read log file and deduct last payment if there was error in last payment
void checkAndDeduct()
{
  Serial.print("Proccesing last payment Please waitttttt........");
  struct logData x = readFile();
  if (x.userID != "null" && x.energy > 0 && x.isPaymentDone == "NO")
  {
    Serial.println("Processing last payment");
    deductMoney(x.energy , x.userID ,String(x.transactionID));
  }
  else
  {
    Serial.println("Last payment was succesfull");
  }
}

// read file
struct logData readFile()
{
  struct logData data;
  String dataa[200];
  char *strings[6]; // an array of pointers to the pieces of the above array after strtok()
  char *ptr = NULL;
  String userID = "--";
  String power = "--";
  String voltage = "--";
  String current = "--";
  String energy = "--";
  String time = "--";
  String transactionID = "--";
  String error = "--";
  String isPaymentDone = "--";
  String limitTocharge = "--";
  File myFile = SD.open("/chargerLogs.txt", FILE_READ); // file open, in read modus
  Serial.println(myFile.size());                        // just to see the filesize...
  Serial.println("Reading File");
  int aantal = 0;
  if (myFile.size() > 400)
  {
    aantal = myFile.size() - 200;
  }
  myFile.seek(aantal); // the cursor goes to 100 regels before the end-of-file
  for (int i = 0; i < 20; i++)
  {                                               // now we go read the next 100 lines
    String buffer = myFile.readStringUntil('\n'); // String read in "buffer"
    dataa[i] = buffer;
  }
  for (int i = 0; i < 20; i++)
  {
    if (dataa[i].length() > 0)
    {
      int ind1 = dataa[i].indexOf(',');           // finds location of first ,
      int ind2 = dataa[i].indexOf(',', ind1 + 1); // finds location of second ,
      int ind3 = dataa[i].indexOf(',', ind2 + 1);
      int ind4 = dataa[i].indexOf(',', ind3 + 1);
      int ind5 = dataa[i].indexOf(',', ind4 + 1);
      int ind6 = dataa[i].indexOf(',', ind5 + 1);
      int ind7 = dataa[i].indexOf(',', ind6 + 1);
      int ind8 = dataa[i].indexOf(',', ind7 + 1);
      int ind9 = dataa[i].indexOf(',', ind8 + 1);
      int ind0 = dataa[i].indexOf(',', ind9 + 1);
      if (ind1 != -1 && ind2 != -1 && ind3 != -1 && ind4 != -1 && ind5 != -1 && ind6 != -1 && ind7 != -1 && ind8 != -1 && ind9 != -1)
      {
        time = dataa[i].substring(0, ind1);              // captures first data String
        userID = dataa[i].substring(ind1 + 1, ind2 + 1); // captures second data String
        energy = dataa[i].substring(ind2 + 1, ind3 + 1);
        voltage = dataa[i].substring(ind3 + 1, ind4 + 1); // captures remain part of data after last ,
        power = dataa[i].substring(ind4 + 1, ind5 + 1);
        current = dataa[i].substring(ind5 + 1, ind6 + 1);
        error = dataa[i].substring(ind6 + 1, ind7 + 1);
        transactionID = dataa[i].substring(ind7 + 1, ind8 + 1);
        limitTocharge = dataa[i].substring(ind8 + 1, ind9 + 1);
        isPaymentDone = dataa[i].substring(ind9 + 1);
        time.replace(',', ' ');
        energy.replace(',', ' ');
        userID.replace(',', ' ');
        voltage.replace(',', ' ');
        power.replace(',', ' ');
        current.replace(',', ' ');
        transactionID.replace(',', ' ');
        error.replace(',', ' ');
        isPaymentDone.replace(',', ' ');
        limitTocharge.replace(',', ' ');
      }
    }
  }
  data.time = time;
  data.userID = userID;
  data.energy = energy.toFloat();
  data.power = power.toFloat();
  data.current = current.toFloat();
  data.voltage = voltage.toFloat();
  data.transactionID = transactionID;
  data.error = error;
  data.isPaymentDone = isPaymentDone;
  data.limitTocharge = limitTocharge.toInt();
  Serial.println("Data read done");
  return data;
}

// store logs to memory card module
void logSDCard(String time, String userID, float energy, float voltage, float power, float current, String error, String transactionID, int limitTocharge, String isPaymentDone)
{
  String dataMessage = String(time) + "," + String(userID) + "," + String(energy) + "," + String(voltage) + "," + String(power) + "," + String(current) + "," + String(error) + "," + String(transactionID) + "," + String(limitTocharge) + "," + String(isPaymentDone) + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *message)
{
  Serial.printf("Writing file: /chargerLogs.txt\n");

  File file = fs.open("/chargerLogs.txt", FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

// mqtt connect function for gsm module
boolean mqttConnect()
{
  Serial.print("Connecting to ");
  Serial.print(mqtt_server);

  // Connect to MQTT Broker without username and password
  // boolean status = mqtt.connect("GsmClientN");

  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect("GsmClientN");

  if (status == false)
  {
    Serial.println(" fail");
    connectTOGSM();
    return false;
  }
  Serial.println("success");
  mqtt.subscribe(deviceID);
  return mqtt.connected();
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
  String lmt = doc["limit"];
  String bal = doc["card_balance"];
  String year = doc["year"];
  String month = doc["month"];
  String day = doc["date"];
  String hour = doc["hour"];
  String minute = doc["minutes"];
  String seconds = doc["seconds"];
  if (year.toInt() != 0 && month.toInt() != 0 && day.toInt() != 0)
  {
    rtc.adjust(DateTime(year.toInt(), month.toInt(), day.toInt(), hour.toInt(), minute.toInt(), seconds.toInt()));
  }

  if (busyWith == "null")
  {
    if (card != "null")
    {
      verifyUserAndStartCharging(card, lmt.toInt());
    }
    else
    {
      Serial.print("Value not found");
    }
  }
  Serial.print(card);
}

// Display status of charger
void displayStatus(void *parameters)
{
  for (;;)
  {
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    delay(200);
    lcd.print(line1);
    lcd.home();
    delay(100);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    delay(100);
    lcd.setCursor(0, 2);
    lcd.print(line3);
    delay(100);
    lcd.setCursor(0, 3);
    lcd.print(line4);
    ledcWrite(redChannel, red);
    ledcWrite(greenChannel, green);
    ledcWrite(blueChannel, blue);
    delay(2300);
  }
}
// connect to GPRS
void connectTOGSM()
{
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(6000);
  Serial.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3)
  {
    modem.simUnlock(GSM_PIN);
  }
  Serial.print("Connecting to APN: ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass))
  {
    Serial.println(" fail");
    useGSM = false;
  }
  else
  {
    Serial.println(" OK");
  }

  if (modem.isGprsConnected())
  {
    useGSM = true;
    Serial.println("GPRS connected");
  }
}

// this function will make sure that wifi is connected or not if not then it will connect to wifi again in case of any failure
void connectToWiFi(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("wifi connected already");
      Serial.print("Wifi ip address is : ");
      Serial.println(WiFi.localIP());
      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    else
    {
      Serial.println("Connecting to wifi");
      if(!modem.isGprsConnected()){
      line1 = "Connecting to wifi";
      line2 = "Please wait...";
      }
      WiFi.begin(ssid, password);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      client.setServer(mqtt_server, mqtt_port);
      client.setCallback(callback);
    }
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by wifi connection is : ");
    Serial.println(uxHighWaterMark);
  }
}

// create function for energy monitoring and saving log data
void energyMetering(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  int count = 0; // set this count to 0  and check for 2 times 0 of current and turn off the relay if current is low more then 5 times
  for (;;)
  {
    struct physicalPerameters readings = readPzem();
    if (client.connected())
    {
      String data = "{\"busyWith\":\"" + busyWith + "\",\"voltage\":\"" + readings.voltage + "\",\"current\":\"" + readings.current + "\",\"energy\":\"" + readings.energy + "\",\"power\":\"" + readings.power + "\"}";
      client.publish(deviceID, data.c_str());
    }
    if (mqtt.connected())
    {
      String data = "{\"busyWith\":\"" + busyWith + "\",\"voltage\":\"" + readings.voltage + "\",\"current\":\"" + readings.current + "\",\"energy\":\"" + readings.energy + "\",\"power\":\"" + readings.power + "\"}";
      mqtt.publish(deviceID, data.c_str());
    }
    // line1 = readings.voltage + "V " + readings.current + "amp";
    line2 = "Unit: " + readings.energy;
    struct logData x = readFile();
    if (busyWith == "null")
    {
      x.transactionID = "null";
      line3 = "V: "+readings.voltage + " U: " + readings.energy;
      line4 = "Anemoi Technologies";
    }
    else
    {
      line1 = busyWith;
    }
    logSDCard(getTimeNow(), busyWith, readings.energy.toFloat(), readings.voltage.toFloat(), readings.power.toFloat(), readings.current.toFloat(), "Error name", String(x.transactionID), limit, "NO");
    if (limit <= readings.energy.toFloat())
    {
       digitalWrite(relay , LOW);
         if(busyWith != "null"){
          deductMoney(readings.energy.toFloat() , busyWith ,String(x.transactionID));
          pzem.resetEnergy();
          }
    }
    if (readings.current.toFloat() == 0.0)
    {
      count = count + 1;
    }
    else
    {
      count = 0;
    }
    if (count >= 2)
    {
      digitalWrite(relay, LOW);
      if (busyWith != "null")
      {
        deductMoney(readings.energy.toFloat(), busyWith, String(x.transactionID));
        count = 0;
        busyWith = "null";
        pzem.resetEnergy();
      }
    }
    if (busyWith == "null")
    {
      digitalWrite(relay, LOW);
      line1 = "Welcome!";
      blue = 0;
      green = 255;
      red = 0;
      count = 0;
    }
    else if (readings.current.toFloat() < 0.01 && readings.current.toFloat() > 0)
    {
      blue = 255;
      green = 0;
      red = 255;
    }
    else
    {
      blue = 255;
      green = 0;
      red = 0;
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by energy metering is : ");
    Serial.println(uxHighWaterMark);
  }
}

// This function check weather device is connected to MQTT broker or not if not then it will connect to mqtt
void connectToMQTT(void *parameters)
{
  UBaseType_t uxHighWaterMark;
  for (;;)
  {
    if (modem.isGprsConnected())
    {
      if (!mqtt.connected())
      {
        Serial.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        boolean isConnectedtoMQTTGSM = mqttConnect();
        if (isConnectedtoMQTTGSM)
        {
          mqtt.loop();
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
      }
      else
      {
        mqtt.loop();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }
    else
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        if (!client.connected())
        {
          // Serial.print("Attempting MQTT connection...");
          //  Create a random client ID
          String clientId = "ESP32Client-";
          // Attempt to connect
          client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD);
          Serial.println("connected");
          // Once connected, publish an announcement...
          client.publish(deviceID, clientId.c_str());
          // ... and resubscribe
          client.subscribe(deviceID);
          vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
        else
        {
          Serial.println("MQTT already connected");
          vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        // get messages from server
        client.loop();
      }
      else
      {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
    }
    if(client.connected() || mqtt.connected()  ){
          blue = 0;
          green = 255;
          red = 0;
     }else{
       blue = 0;
       green = 0;
       red = 255;
       line1 = "Server not found";
      }
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by MQTT handler is : ");
    Serial.println(uxHighWaterMark);
  }
}

// detect card
void detect_card(void *parameters)
{
  /* function readRFID */
  UBaseType_t uxHighWaterMark;

  for (;;)
  {
    readRFID();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("maxdata used by rfid reader is : ");
    Serial.println(uxHighWaterMark);
  }
}

void readRFID(void)
{ /* function readRFID */
  // Read RFID card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

// Function to convert Binary to Decimal
long convertBinaryToDecimal(long binary)
{
  long number = binary;
  long decimalVal = 0;
  long baseVal = 1;
  long tempVal = number;
  long previousDigit;
  while (tempVal)
  {
    // Converts Binary to Decimal
    previousDigit = tempVal % 10;
    tempVal = tempVal / 10;
    decimalVal += previousDigit * baseVal;
    baseVal = baseVal * 2;
  }
  // Returns the Decimal number
  return decimalVal;
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
char printDec(byte *buffer, byte bufferSize)
{
  String rfid = "";
  long size1 = 0;
  for (byte i = 0; i < bufferSize; i++)
  {
    size1 = convertBinaryToDecimal(bufferSize);
    long value = convertBinaryToDecimal(buffer[i]);
    rfid += String(value);
  }
  Serial.println(size1);
  Serial.println(rfid);

  if (busyWith == "null")
  {
    if (rfid != "null")
    {
      verifyUserAndStartCharging(rfid, 1000);
    }
    else
    {
      Serial.print("Value not found");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);
  pzem.resetEnergy();
  xTaskCreatePinnedToCore(
      displayStatus,               // function name which you want to run
      "Display status of charger", // function description
      3000,                        // stack size
      NULL,                        // parameters of function
      1,                           // task priority
      NULL,                        // task handler
      0                            // in which core you want to run this tast either 1 or 0
  );
  spiSD.begin(14, 12, 13, 15); // SCK,MISO,MOSI,SS //HSPI1
  while (!SD.begin(SD_CS, spiSD))
  {
    blue = 0;
    green = 0;
    red = 255;
    line1 = "SD card error";
  }
    while (!rtc.begin())
  {
    red = 255;
    blue = 0;
    green = 0;
    line1 = "RTC not found";
  }
  while (!rtc.isrunning())
  {
    blue = 0;
    green = 0;
    red = 255;
    line1 = "RTC error";
  }
  
  blue = 255;
  green = 255;
  red = 255;
  ledcWrite(redChannel, red);
  ledcWrite(greenChannel, green);
  ledcWrite(blueChannel, blue);
  connectTOGSM();
  // MQTT Broker setup
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);
  SPI.begin();
  rfid.PCD_Init();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();

  // sd card module
  File file = SD.open("/chargerLogs.txt");
  if (!file)
  {
    writeFile(SD, "Time , User ID , Energy , Voltage , Power , Current , Error , TransactionID , LimitTocharge , isPaymentDone \r\n");
  }
  else
  {
    // Serial.println("File already exists");
  }
  file.close();
  blue = 0;
  green = 255;
  red = 0;
  ledcWrite(redChannel, red);
  ledcWrite(greenChannel, green);
  ledcWrite(blueChannel, blue);
  // create task for wifi connectivity
    xTaskCreatePinnedToCore(
        connectToWiFi,         // function name which you want to run
        "Connect to wifi",     // function description
        10000,                  // stack size
        NULL,                  // parameters of function
        1,                     // task priority
        &connectToWiFiHandler, // task handler
        1                      // in which core you want to run this tast either 1 or 0
    );
  checkAndDeduct();
  // create task for mqtt connectivity
  delay(1000);
  xTaskCreatePinnedToCore(
      connectToMQTT,            // function name which you want to run
      "Connect to MQTT server", // function description
      12000,                    // stack size
      NULL,                     // parameters of function
      1,                        // task priority
      &connectToMQTTHandler,    // task handler
      1                         // in which core you want to run this tast either 1 or 0
  );
  delay(1000);
  // create a task for energy meatering
  xTaskCreatePinnedToCore(
      energyMetering,          // function name which you want to run
      "Energy Meatering",      // function description
      15000,                    // stack size
      NULL,                    // parameters of function
      1,                       // task priority
      &energyMeateringHandler, // task handler
      0                        // in which core you want to run this tast either 1 or 0
  );
 delay(1000);
  // create task for reading rfid card
  xTaskCreatePinnedToCore(
      detect_card,          // function name which you want to run
      "detect rfID card",   // function description
      15000,                // stack size
      NULL,                 // parameters of function
      1,                    // task priority
      &readRfidCardHandler, // task handler
      0                     // in which core you want to run this tast either 1 or 0
  );

  // put your setup code here, to run once:
}

void loop()
{
  // vTaskResume(taskHandler1);
  // put your main code here, to run repeatedly:
}
