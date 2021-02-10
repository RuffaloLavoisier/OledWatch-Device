void menu() {
  char* MENU_TITLE[] = {"[ BACK ]", "LED", "LASER", "ALARM", "TEMP", "GPS", "BLUETOOTH", "ABOUT"};

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(centerX - 64, centerY - 32);
  for (int count = 0; count < 8; count++) {//draw 0-7 title
    if (count == 4 && pointer_menu > 3) { //other page  count-4 and pointer menu > this is 3
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(centerX - 64, centerY - 32);
    }

    if (pointer_menu == count)//pointer and menu count match
    {

      display.print(">");
      //------------------------------------------
      if (LOW == digitalRead(button_center)) {
        Delay(100);
        switch (pointer_menu) { //select back
          case 0://back
            state = false;
            break;
          case 1://led
            break;
          case 2://laser
            break;
          case 3://select alarm
            //alarm_state = !alarm_state;
            //digitalWrite(ALARM, alarm_state);
            
            alarm_clock();
            break;
          case 4://select temp
            temp_monitor();
            break;
          case 5:
            break;
          case 6:
            break;
          case 7://select info
            show_info();
            break;
        }
      }
      //------------------------------------------
    }
    else//non pointer
    {
      display.print(" ");
    }
    display.println(MENU_TITLE[count]);
    display.println();
  }
  display.display();
}
