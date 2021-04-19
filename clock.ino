void temp_monitor() {

  bool buttonhit = false;
  bool ok_button = false;
  Serial.print(" in");
  while (1) {
    //------------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(centerX - 46, centerY + 6);
    display.print("TEMP : ");
    display.print(rtc.getTemperature());
    display.println(" C");
    display.println(" ");
    display.println(" ");
    display.println(" ");
    display.println(" ");
    display.println(" ");
    display.display();

    //------------------
    if (digitalRead(button_center) == HIGH) {
      ok_button = true;
      Serial.print("  ok btn");
      Serial.print(ok_button);
    }
    if (digitalRead(button_center) == LOW && ok_button == true) {
      buttonhit = true;

      Serial.print("  buttonhit");
      Serial.print(buttonhit);
    }

    else if (buttonhit == true && digitalRead(button_center) == HIGH) {
      Serial.println("  break");
      break;
    }
  }
}
void show_info() {
  bool buttonhit = false;
  bool ok_button = false;
  while (1) {

    //------------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(centerX - 64, centerY - 32);
    display.println("MADE");
    display.println(" ");
    display.println("Jeon Sunghwan");
    display.println(" ");
    display.println("VER.1.3");
    display.println(" ");
    display.print("LAST "); display.print(dev_y); display.print("-"); display.print(dev_m); display.print("-"); display.println(dev_d);
    display.println(" ");
    display.display();

    //------------------
    if (digitalRead(button_center) == HIGH)ok_button = true;
    if (digitalRead(button_center) == LOW && ok_button == true) {
      buttonhit = true;
    }

    else if (buttonhit == true && digitalRead(button_center) == HIGH) {
      break;
    }
  }
}
void analog_clock(int h, int m, int s, int M, int D) {
  // CLOCK_STYLE_SIMPLE_ANALOG.
  display.drawCircle(centerX, centerY, Radius, WHITE);
  showTimePin(centerX, centerY, 0.1, 0.5, h * 5 + (int)(m * 5 / 60));
  showTimePin(centerX, centerY, 0.1, 0.78, m);
  showTimePin(centerX, centerY, 0.1, 0.9, s);
  display.display();

}
void dual_clock(int h, int m, int s, int M, int D) {
  //KOREA
  display.drawCircle(centerY, Radius - 6, Radius - 6 , WHITE);
  showTimePin(centerY, Radius - 6, 0.1, 0.4, h * 5 + (int)(m * 5 / 60));
  showTimePin(centerY, Radius - 6, 0.1, 0.6, m);
  showTimePin(centerY, Radius - 6, 0.1, 0.75, s);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(centerY - 5, display.height() - 9);
  display.print("KR");
  if (h >= 12 && h <= 23)//PM
    display.print("*");
  else if (h >= 0 && h <= 11) //AM
    display.print(" ");
  //US

  display.drawCircle(display.width() - centerY, Radius - 6, Radius - 6 , WHITE);
  showTimePin(display.width() - centerY, Radius - 6, 0.1, 0.4, US_h * 5 + (int)(US_m * 5 / 60));
  showTimePin(display.width() - centerY, Radius - 6, 0.1, 0.6, US_m);
  showTimePin(display.width() - centerY, Radius - 6, 0.1, 0.75, US_s);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(display.width() - centerY - 5, display.height() - 9);
  display.print("US");
  if (US_h >= 12 && US_h <= 23)//PM
    display.print("*");
  else if (US_h >= 0 && US_h <= 11) //AM
    display.print(" ");

  display.display();
}
void full_text_clock(int h, int m, int s, int M, int D) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(centerX - 41, centerY - 17);
  if (M < 10)
    display.print("0");
  display.print(M);
  display.print(" / ");
  if (D < 10)
    display.print("0");
  display.println(D);
  //display.setTextSize(2);
  //display.setCursor(centerX + 11, centerY - 17);
  //display.println((const char*)pgm_read_word(&(ampmString[iAmPm])));

  display.setTextSize(2);
  display.setCursor(centerX - 46, centerY + 6);
  if (h < 10)
    display.print("0");
  display.print(h);
  display.print(":");
  if (m < 10)
    display.print("0");
  display.print(m);
  display.print(":");
  if (s < 10)
    display.print("0");
  display.println(s);
  display.display();
}
void mix_clock(int h, int m, int s, int M, int D, int DN) {
  display.drawCircle(centerY, centerY, Radius - 6 , WHITE);
  showTimePin(centerY, centerY, 0.1, 0.4, h * 5 + (int)(m * 5 / 60));
  showTimePin(centerY, centerY, 0.1, 0.70, m);
  // showTimePin(centerX, centerY, 0.1, 0.9, s);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(centerY * 2 + 3, 23);
  display.print(daysOfTheWeek[DN]);
  display.setCursor(centerY * 2 + 3, 9);
  if (M < 10)
    display.print("0");
  display.print(M);
  display.print(" / ");
  if (D < 10)
    display.print("0");
  display.println(D);
  //display.println((const char*)pgm_read_word(&(weekString[iWeek])));
  //display.setCursor(centerY * 2 + 28, 23);
  //display.println((const char*)pgm_read_word(&(ampmString[iAmPm])));

  display.setTextSize(2);
  display.setCursor(centerY * 2, 37);
  if (h < 10)
    display.print("0");
  display.print(h);
  long HalfTime = millis();
  if (HalfTime - prev >= 500) {
    prev = HalfTime;
    colon = !colon;
  }
  if (colon) display.print(":");
  else display.print(" ");

  display.setCursor(centerY * 2 + 35, 37);
  if (m < 10)
    display.print("0");
  display.println(m);
  display.display();
}
bool alarm_check(int now_h, int now_m) {
  if (alarm_h == now_h && now_m  == alarm_m ) {
    Serial.println("same time is true");
    return true;
  }
  else {
    Serial.println("diff time is false");
    once_state = true;//different time is turn on once state for alarm.
    return false;
  }
}
void alarm_clock() {

  bool buttonhit = false;
  bool ok_button = false;
  while (1) {

    //------------------

    display.clearDisplay();
    display.drawCircle(centerY, centerY, Radius - 6 , WHITE);
    showTimePin(centerY, centerY, 0.1, 0.4, alarm_h * 5 + (int)(alarm_m * 5 / 60));
    showTimePin(centerY, centerY, 0.1, 0.70, alarm_m);
    // showTimePin(centerX, centerY, 0.1, 0.9, s);

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(centerY * 2 + 3, 23);
    if (alarm_h < 10)
      display.print("0");
    display.print(alarm_h);

    long HalfTime = millis();
    if (HalfTime - prev >= 500) {
      prev = HalfTime;
      colon = !colon;
    }
    if (colon) display.print(":");
    else display.print(" ");

    display.setCursor(centerY * 2 + 36, 23);
    if (alarm_m < 10)
      display.print("0");
    display.println(alarm_m);
    display.println();
    display.println();
    display.println();
    display.println();
    display.display();

    //------------------
    if (digitalRead(button_up) == LOW) {
      alarm_m++;
      if (alarm_m > 59)alarm_m = 0;
    }
    else if (digitalRead(button_down) == LOW) {
      alarm_h++;
      if (alarm_h > 23)alarm_h = 0;
    }
    Delay(100);
    if (digitalRead(button_center) == HIGH)ok_button = true;
    if (digitalRead(button_center) == LOW && ok_button == true) {
      buttonhit = true;
      if (once_state == true) {
        once_state = false;
        digitalWrite(ALARM, LOW);
        Serial.println("alarm off");
      }
    }

    else if (buttonhit == true && digitalRead(button_center) == HIGH) {
      break;
    }
  }
}
void DispalyClock(int h, int m, int s, int M, int D, int DN) {

  display.clearDisplay();
  // you can clock style setting
  // if (git_commit_time_check()) { //need commit
  if (1) { //need commit
    GitCommitTime(h, m, s, M, D, DN);
  }
  else { //normal
    switch (clockStyle) {
      case 1:
        mix_clock(h, m, s, M, D, DN);
        break;
      case 2:
        full_text_clock(h, m, s, M, D);
        break;
      case 3:
        dual_clock(h, m, s, M, D);
        break;
      case 4:
        analog_clock(h, m, s, M, D);
        break;
    }
  }


}

bool git_commit_time_check() {
  if (Morning || Lunch || Dinner)
    return true;
  else
    return false;
}

void GitCommitTime(int h, int m, int s, int M, int D, int DN) {
  //analog watch
  display.drawCircle(centerX, centerY / 2, Radius - 10 , WHITE);
  showTimePin(centerY, centerY, 0.1, 0.4, h * 5 + (int)(m * 5 / 60));
  showTimePin(centerY, centerY, 0.1, 0.70, m);
  // showTimePin(centerX, centerY, 0.1, 0.9, s);

  //day name
  /*  display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(centerY * 2 + 3, 23);
    display.print(daysOfTheWeek[DN]);
  */
  //  timer algorithm

  //setting commit close time
  if      (Morning) CloseCommitTime = MORNING_CLOSE;
  else if (Lunch)   CloseCommitTime = LUNCH_CLOSE;
  else if (Dinner)  CloseCommitTime = DINNER_CLOSE;

  //cal remain time

  //10:0:0 - 13:0:0 * 11:0:0 , 22:0:0 - 2:0:0 * 23:0:0
  
  /***initialize value***/
  // 목표 값에 현재 시간을 넣으면 남은 시간을 출력



   /********calculate timer for remain*********/

  if(now.second()==0)
  { 
    remain_sec = 0;
    enable_59 = 1;
  }
  else
  { 
    if(enable_59)
    {
      remain_min = remain_min - 1;
      if( remain_min < 0 )
      {
        remain_min = 59;
        remain_hr = remain_hr - 1;
      }
      enable_59 = 0;
    }
    remain_sec = 60 - now.second();
  }

   

 
  //date
  /*
    display.setCursor(centerY * 2 + 3, 9);
    if ( M < 10 ) display.print("0");
    display.print(M);
    display.print(" / ");
    if ( D < 10 ) display.print("0");
    display.println(D);
  */

  //display.println((const char*)pgm_read_word(&(weekString[iWeek])));
  //display.setCursor(centerY * 2 + 28, 23);
  //display.println((const char*)pgm_read_word(&(ampmString[iAmPm])));

  //digital watch
  display.setTextSize(2);
  display.setCursor(centerX - 46, centerY + 6); // x,y -> 128,64
  if (remain_hr < 10)
    display.print("0");
  display.print(remain_hr);
  display.print(":");
  if (remain_min < 10)
    display.print("0");
  display.print(remain_min);
  display.print(":");
  if (remain_sec < 10)
    display.print("0");
  display.println(remain_sec);
  display.display();
}
void print_RSSI() {
  //1.연결 확인 후 바로 RSSI 값을 출력
  //2.휴대폰에서 RSSI 명령어를 치면 시계에도 출력

}