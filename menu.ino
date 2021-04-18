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

    if (pointer_menu == count)//pointer and menu count match >> count : draw menu number , pointer menu : control user from button
    {
      display.print(">");//draw pointer " > "
      //------------------------------------------
      if (LOW == digitalRead(button_center)) {
        Delay(100);//non stop board .still running !
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
            temp_monitor(); //do you need this menu ?
            break;
          case 5://GPS
            break;
          case 6://BLUETOOTH
            print_RSSI();
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
      display.print(" ");//just print title
    }
    display.println(MENU_TITLE[count]);//print menu title
    display.println();
  }
  display.display();
}
