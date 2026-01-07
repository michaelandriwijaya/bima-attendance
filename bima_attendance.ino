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

///////////* LIBRARY START *///////////
// library for LCD display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// library for MFRC522 (rfid card reader)
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// library for wifi and ntp (obtaining current time)
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// library for goggle sheet
#include <ESP_Google_Sheet_Client.h>
///////////* LIBRARY END *///////////

///////////* DEFINE START *///////////
// port assignment
int port_buzzer = 2;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// rfid setting
// Learn more about using SPI/I2C or check the pin assigment for your board: https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
MFRC522DriverPinSimple ss_pin(15);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
//MFRC522DriverI2C driver{};     // Create I2C driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

//for rfid reading
MFRC522::MIFARE_Key key;
byte blockAddress = 2;
byte bufferblocksize = 18; // 16 bytes data and 2 bytes crc
byte blockDataRead[18];

// Wi-fi setup
const char* ssid     = "*****"; // write your ssid here
const char* password = "*****"; // write your password here

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variables to save date and time
time_t epochTime;
String formattedTime;
int currentHour;
int currentMinute;   
int currentSecond;
String weekDay;
int monthDay;
int currentMonth;
String currentMonthName;
int currentYear;
String currentDate;

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Google Project ID
#define PROJECT_ID "write your project ID"

// Service Account's client email
#define CLIENT_EMAIL "write your service account client email"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY----- your key here -----END PRIVATE KEY-----\n";

// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "writer your spreadsheet ID";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 10000;

// Token Callback function
void tokenStatusCallback(TokenInfo info);

// status sending to gsheet
bool isSendingGsheet = false;
///////////* DEFINE END *///////////

///////////* METHOD START *///////////
void tokenStatusCallback(TokenInfo info){
    if (info.status == token_status_error){
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else{
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}
///////////* METHOD END *///////////

void setup() {
  // initialize buzzer port
  pinMode(port_buzzer, OUTPUT);

  // initialize lcd and serial connection
  lcd_serial_init();

  // initialize rfid
  rfid_init();

  // initialize wifi
  WiFi.begin(ssid, password);
  lcd_serial_wifi_init();
  wifi_connect();

  // initialize date
  lcd_serial_date_init();
  // Initialize a NTPClient to get time
  time_init();

  // initialize google sheet
  gsheet_init();

  lcd_serial_system_ready();

  lcd_serial_bima_greeting();

  buzzer_bipbip();
}

void loop() {
  // if currently sending sheet, loop until finished
  if(isSendingGsheet)
  {
    gsheet_update();
    return;
  }

  // Check if a new card is present
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }
  rfid_detect();
  lcd_serial_welcome_message();

  time_update();
  lcd_serial_time_update();

  isSendingGsheet = true;
  lcd_serial_updating();
}