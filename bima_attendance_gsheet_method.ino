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
void gsheet_init(){
  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);

  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

void gsheet_update(){
  // Call ready() repeatedly in loop for authentication checking and processing
  bool ready = GSheet.ready();
  if (ready && millis() - lastTime > timerDelay){
    lastTime = millis();
  
    FirebaseJson response;

    Serial.println("\nAppend spreadsheet values...");
    Serial.println("----------------------------");

    FirebaseJson valueRange;

    String employeeName = "";
    String knownData1 = "*****";
    // put data here

    //char compareName[16];
    String readData = "";
    for (byte i = 0; i < 16; i++) {
      readData += (char)blockDataRead[i]; // Convert byte to char for string comparison
    }
    
    if (readData.startsWith(knownData1))
    {
      employeeName = "john doe";
    } 
    // insert all employees
    else 
    {
      employeeName = "???";
    }

    valueRange.add("majorDimension", "COLUMNS");
    valueRange.set("values/[0]/[0]", currentDate);
    valueRange.set("values/[1]/[0]", formattedTime);
    valueRange.set("values/[2]/[0]", employeeName);

    // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/append
    // Append values to the spreadsheet
    bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append */, &valueRange /* data range to append */);
    if (success){
        response.toString(Serial, true);
        valueRange.clear();
    }
    else{
        Serial.println(GSheet.errorReason());
        lcd.println("Error! Coba lagi!");
    }
    Serial.println();
    Serial.println(ESP.getFreeHeap());

    isSendingGsheet = false;
        
    // update lcd
    lcd_serial_bima_greeting();
  }
  return;
}
///////////* METHOD END *///////////