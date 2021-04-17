//2021-01-20 menu ui update
//2021-01-20 dual clock
//2021-01-23 alarm
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include "RTClib.h"
#include <timer.h>

#define BUZZER 12
#define ALARM 13
#define OLED_RESET 7 // not used
#define PIXEL_COUNT 1
#define PIXEL_PIN   6

auto timer = timer_create_default();
Adafruit_SH1106 display(OLED_RESET);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

RTC_DS3231 rtc;

unsigned long prev = 0;
bool colon = false;

char* daysOfTheWeek[] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

uint8_t centerX = 64;
uint8_t centerY = 32;
uint8_t Radius = 28;

int     dev_y = 0;
uint8_t dev_m = 0;
uint8_t dev_d = 0;

uint8_t US_h = 0;
uint8_t US_m = 0;
uint8_t US_s = 0;

uint8_t clockStyle = 3;

uint8_t led = 0;
uint8_t laser = 13;
//---------pin setting----------- 

/*
    ****BOTTOM*****
    vibe
    xshut
    I2C

    ***TOP***
    AD0
    DP5
    led alarm
    buzzer
    I2C

*/
uint8_t alarm_h = 3;
uint8_t alarm_m = 53;

uint8_t button_up = 10;
uint8_t button_center = 9;
uint8_t button_down = 8;

int pointer_menu = 0;//where is pointer 

bool once_state = false;
bool menu_state = HIGH;
bool state = false;

void setup() {
  Serial.begin(9600);               //Serial Debug
  Serial1.begin(9600);              //Bluetooth start !
 
  centerX = display.width() / 2;    //find center value
  centerY = display.height() / 2;   //find center value
  Radius  = centerY - 2;            //fine rad value
  
  if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) 
  {
    // this will adjust to the date and time at compilation
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //  rtc.disable32K();

  timer.every(9000, keypower);


  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_center, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(laser, OUTPUT);;
  pinMode(ALARM, OUTPUT);
  pinMode(PIXEL_PIN, OUTPUT);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  //delay(2000);

  DateTime now = rtc.now();
  dev_y = now.year();
  dev_m = now.month();
  dev_d = now.day();

}

void loop() {

  timer.tick();
  DateTime now = rtc.now();

  DateTime US (now - TimeSpan(0, 14, 0, 18));
  US_h = US.hour();
  US_m = US.minute();
  US_s = US.second();
  //temp_monitor();

  while (digitalRead(button_center) == LOW)menu_state = LOW;
  Delay(100);
  while (digitalRead(button_center) == LOW)menu_state = LOW;

  if (menu_state == LOW) //center check
  {
    state = !state;
    menu_state = HIGH;
  }

  if (state == true) 
  {
    pointer_menu = 1;
    while (state == true) 
    {
      timer.tick();
      if (LOW == digitalRead(button_up))
      {
        pointer_menu--;
        if (pointer_menu < 0)pointer_menu = 7;
      }
      else if (LOW == digitalRead(button_down))
      {
        pointer_menu++;
        if (pointer_menu > 7)pointer_menu = 0;
      }
      menu();
      //colorWipe(strip.Color(  0, 255,   0), 50); // Green
      //     if (alarm_check(now.hour(),now.minute()) && once_state) {
      //        Serial.println("alarm check and once state true ");
      //      state = false;
      //    break;
      // }
      Delay(100);
    }
  }
  else if (state == false)
  {
    if (LOW == digitalRead(button_up)) 
    {
      Serial.println(clockStyle);
      clockStyle = clockStyle + 1;
      if (clockStyle > 4)clockStyle = 1;
    }
    DispalyClock(now.hour(), now.minute(), now.second() , now.month(), now.day(), now.dayOfTheWeek());
    //colorWipe(strip.Color(  0,   0,   0), 50); // Black/off
    Delay(100);
    if (alarm_check(now.hour(), now.minute()) && once_state) 
    {
      Serial.println("alarm on");
      digitalWrite(ALARM, HIGH);
    }
  }
}
//----------------------------------
void Delay(int delay_time) {//if you need delay without stop just stay here
  long prev = millis();
  while (millis() - prev < delay_time);//stay here
}

//thank you google !
void showTimePin(int center_x, int center_y, double pl1, double pl2, double pl3) //need display show time pin
{
  double x1, x2, y1, y2;
  double RAD = 3.141592 / 180;
  double LR = 89.99;
  x1 = center_x + (Radius * pl1) * cos((6 * pl3 + LR) * RAD);
  y1 = center_y + (Radius * pl1) * sin((6 * pl3 + LR) * RAD);
  x2 = center_x + (Radius * pl2) * cos((6 * pl3 - LR) * RAD);
  y2 = center_y + (Radius * pl2) * sin((6 * pl3 - LR) * RAD);

  display.drawLine((int)x1, (int)y1, (int)x2, (int)y2, WHITE);
}

//      how to use buzzer
//      digitalWrite(BUZZER, HIGH);
//      digitalWrite(BUZZER, LOW);

//  colorWipe(strip.Color(255,   0,   0), 50); // Red
//  colorWipe(strip.Color(  0, 255,   0), 50); // Green
//  colorWipe(strip.Color(  0,   0, 255), 50); // Blue
//  colorWipe(strip.Color(  0,   0,   0), 50); // Black/off
void colorWipe(uint32_t c, uint8_t wait) {//led
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void keypower() {//stay operatr
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(500);
  pinMode(4, INPUT);
}
