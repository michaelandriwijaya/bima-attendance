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
void lcd_serial_init(){
  Wire.begin();
  Serial.begin(115200);  // Initialize serial communication
  while (!Serial);       // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).

   // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // post status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Serial Started!");

  Serial.println("Serial Started!");

  delay(500);
}

void lcd_serial_wifi_init(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init Wifi");

  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  delay(500);
}

void lcd_serial_wifi_wait(int i){
  // no lcd.clear()
  lcd.setCursor(i, 1);
  lcd.print(".");
  
  Serial.print(i+1); Serial.print(' ');

  delay(500);
}

void lcd_serial_wifi_connected(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wifi connected!");

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  delay(500);
}

void lcd_serial_date_init(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Init Date");

  Serial.println('\n');
  Serial.println("Init Date");  

  delay(500);
}

void lcd_serial_gsheet_init(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Init GSheet");

  Serial.println('\n');
  Serial.println("Init GSheet");  

  delay(500);
}

void lcd_serial_system_ready(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Ready!");

  Serial.println('\n');
  Serial.println("Ready!");  

  delay(500);
}

void lcd_serial_bima_greeting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("--- Toko Bima---");
  lcd.setCursor(0,1);
  lcd.print("Scan kartu anda");
}

void lcd_serial_updating(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tunggu sebentar");
  lcd.setCursor(0, 1);
  lcd.print("Diunggah");
}

void lcd_serial_welcome_message(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Selamat Datang:");
  lcd.setCursor(0,1);
  for (byte i = 4; i < 16; i++) {// skip the number 
    lcd.print((char)blockDataRead[i]);
  }
  buzzer_bip();
  delay(2000);
}

void lcd_serial_time_update(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(currentDate);
  lcd.setCursor(0,1);
  lcd.print(formattedTime);

  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  
  Serial.print("Hour: ");
  Serial.println(currentHour);  
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  
  Serial.print("Week Day: ");
  Serial.println(weekDay);    
  Serial.print("Month day: ");
  Serial.println(monthDay);
  Serial.print("Month: ");
  Serial.println(currentMonth);
  Serial.print("Month name: ");
  Serial.println(currentMonthName);
  Serial.print("Year: ");
  Serial.println(currentYear);
  Serial.print("Current date: ");
  Serial.println(currentDate);
  Serial.println("");

  delay(2000);
}
///////////* METHOD END *///////////