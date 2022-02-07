void menu()
{
  char *MENU_TITLE[] = {"[ BACK ]", "LED", "PUSH", "ALARM", "TEMP", "GPS", "BLUETOOTH", "ABOUT"};

  display.clearDisplay();
  display.setTextSize(1); //제일 작은 사이즈
  display.setTextColor(WHITE);
  //display.setCursor(centerX - 64, centerY - 32);
  display.setCursor(centerX - centerX, centerY - centerY); //시작점
  for (int count = 0; count < sizeof(MENU_TITLE) / sizeof(char *); count++)
  { //draw 0-7 title  //arr 갯수 구하기

    if (count == 4 && pointer_menu > 3)
    { //other page  count-4 and pointer menu > this is 3 ..becaz this menu need next page.
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      //display.setCursor(centerX - 64, centerY - 32);
      display.setCursor(centerX - centerX, centerY - centerY); //시작점
    }

    if (pointer_menu == count)
    { //pointer and menu count match >> count : draw menu number , pointer menu : control user from button

      display.print(">"); // draw pointer " > "

      //------------------------------------------
      if (LOW == digitalRead(button_center))
      {
        Delay(100); //non stop board .still running !
        switch (pointer_menu)
        {      
        case 0: //back
          state = false;
          break;
        case 1: //led
          Towggle_1 = True;
          break;
        case 2: //push
          break;
        case 3: //select alarm
          //alarm_state = !alarm_state;
          //digitalWrite(ALARM, alarm_state);
          alarm_clock();
          break;
        case 4:           //select temp
          temp_monitor(); //do you need this menu ?
          break;
        case 5: //GPS
          break;
        case 6: //BLUETOOTH
          //print_RSSI();
          break;
        case 7: //select info
          show_info();
          break;
        }
      }
      //------------------------------------------
    }
    else{                     //non pointer
      display.print(" "); //just print title
    }

    // Menu choice GUI
    /*
    Code 
    */
    display.println(MENU_TITLE[count]); //print menu title
    display.println();                  // 보기 좋게 글자 배치
  }
  display.display();
}
