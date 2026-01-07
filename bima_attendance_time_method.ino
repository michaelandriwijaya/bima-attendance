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
void time_init(){
  timeClient.begin();
  timeClient.setTimeOffset(25200); // GMT +7
}

void time_update(){
  timeClient.update();

  epochTime = timeClient.getEpochTime();
  formattedTime = timeClient.getFormattedTime();
  currentHour = timeClient.getHours();
  currentMinute = timeClient.getMinutes();   
  currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  monthDay = ptm->tm_mday;
  currentMonth = ptm->tm_mon+1;
  currentMonthName = months[currentMonth-1];
  currentYear = ptm->tm_year+1900;
  //Print complete date:
  currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
}
///////////* METHOD END *///////////