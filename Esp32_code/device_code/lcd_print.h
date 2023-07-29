#include <LiquidCrystal_I2C.h>
// #include "vars.h"
// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;
int lcdInitTime = 31;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x3f, lcdColumns, lcdRows);  
void printOnLcd(String line1, String line2,String line3,String line4){
  if(lcdInitTime >= 30){
  lcd.begin();
  lcd.backlight();
  }
  lcdInitTime++;
  lcd.setCursor(0, 0);
  if (line1.length() > lcdColumns) {
    lcd.print(line1.substring(0, lcdColumns));
  } else {
    lcd.print(line1);
  }
  Serial.println(line2);
  lcd.setCursor(0, 1);
  if (line2.length() > lcdColumns) {
    lcd.print(line2.substring(0, lcdColumns));
  } else {
    lcd.print(line2);
  }

  Serial.println(line3);
  lcd.setCursor(0, 2);
  if (line3.length() > lcdColumns) {
    lcd.print(line3.substring(0, lcdColumns));
  } else {
    lcd.print(line3);
  }

  lcd.setCursor(0, 3);
  if (line4.length() > lcdColumns) {
    lcd.print(line4.substring(0, lcdColumns));
  } else {
    lcd.print(line4);
  }

  // int printed = 0;
  // do{
  //   if(line3.length() > (lcdColumns + printed)){
  //   lcd.setCursor(0, 2);
  //   lcd.print(line3.substring(printed, (line3.length() > lcdColumns ? lcdColumns + printed : line3.length())));
  //   }
  //   if(line4.length() > (lcdColumns + printed)){
  //   lcd.setCursor(0, 3);
  //   lcd.print(line4.substring(printed, (line4.length() > lcdColumns ? lcdColumns + printed : line4.length())));
  //   }
  //   printed++;
  //   vTaskDelay(300 / portTICK_PERIOD_MS);
  // }while(line3.length() > (lcdColumns + printed) || line4.length() > (lcdColumns + printed));

  // if (line4.length() > lcdColumns) {
  //   lcd.print(line4.substring(0, lcdColumns));
  // } else {
  //   lcd.print(line4);
  // }
}