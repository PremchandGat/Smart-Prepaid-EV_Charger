// //Libraries
// #include <SPI.h>//https://www.arduino.cc/en/reference/SPI
// #include <MFRC522.h>//https://github.com/miguelbalboa/rfid
// //Constants
// #define SS_PIN 5
// #define RST_PIN 27
// //Variables
// byte nuidPICC[4] = {0, 0, 0, 0};
// MFRC522::MIFARE_Key key;
// MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

// String readRFID() { /* function readRFID */
//  ////Read RFID card
//  static bool flag  = 0;
//      if(flag == 0){
//       SPI.begin();
//        rfid.PCD_Init();
//       rfid.PCD_DumpVersionToSerial();
//       flag = 1;//https://circuits4you.com/wp-content/uploads/2018/12/ESP32-Pinout.jpg
//      }
     

 
//  for (byte i = 0; i < 6; i++) {
//    key.keyByte[i] = 0xFF;
//  }
//  // Look for new 1 cards
//  if ( ! rfid.PICC_IsNewCardPresent())
//    return "";
//  // Verify if the NUID has been readed
//  if (  !rfid.PICC_ReadCardSerial())
//    return "";
//  // Store NUID into nuidPICC array
//  for (byte i = 0; i < 4; i++) {
//    nuidPICC[i] = rfid.uid.uidByte[i];
//  }
//  String id = "";
//   for (byte i = 0; i < rfid.uid.size; i++) {
//     id += String(rfid.uid.uidByte[i], DEC);
//  }
//  Serial.println("RFID : ");
//  Serial.println(id);
//  // Halt PICC
//  rfid.PICC_HaltA();
//  // Stop encryption on PCD
//  rfid.PCD_StopCrypto1();
//  return id;
// }
