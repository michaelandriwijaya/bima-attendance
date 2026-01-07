/*
  A simple attendance system built from ESP8266 for Toko Bima Bandung
  this system will detect rfid card, get timestamp, send it over wifi to update provided google sheet
  
  A part of this software is based from:
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at 
  https://RandomNerdTutorials.com/esp8266-nodemcu-mfrc522-rfid-reader-arduino/
  https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  https://randomnerdtutorials.com/esp32-datalogging-google-sheets/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

///////////* METHOD START *///////////
void rfid_init(){
  mfrc522.PCD_Init();    // Init MFRC522 board.
  
  // read rfid
  //MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
  //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  
  // Prepare key - all keys are set to FFFFFFFFFFFF at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void rfid_detect(){
  // if card is detected

  // Dump debug info about the card; PICC_HaltA() is automatically called.
  //MFRC522Debug::PICC_DumpToSerial(mfrc522, Serial, &(mfrc522.uid));

  // Display card UID
  Serial.print("----------------\nCard UID: ");
  MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  Serial.println();

  // Authenticate the specified block using KEY_A = 0x60
  if (mfrc522.PCD_Authenticate(0x60, blockAddress, &key, &(mfrc522.uid)) != 0) {
    Serial.println("Authentication failed.");
    return;
  }

  // Read data from the specified block
  if (mfrc522.MIFARE_Read(blockAddress, blockDataRead, &bufferblocksize) != 0) {
    Serial.println("Read failed.");
  } else {
    Serial.println("Read successfully!");
    Serial.print("Data in block ");
    Serial.print(blockAddress);
    Serial.print(": ");
    for (byte i = 0; i < 16; i++) {
      Serial.print((char)blockDataRead[i]);  // Print as character
    }
    Serial.println();
  }
  
  // Halt communication with the card
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
///////////* METHOD END *///////////