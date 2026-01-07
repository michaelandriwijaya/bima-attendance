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
void wifi_connect(){
  int i = 0;

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    lcd_serial_wifi_wait(i);
    i++;
  }

  lcd_serial_wifi_connected();
}
///////////* METHOD END *///////////