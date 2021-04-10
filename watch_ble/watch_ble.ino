/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo and Micro support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>

//SoftwareSerial Serial1(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600);
  Serial1.println("Hello, world?");
}

void loop() { // run over and over
  //연결되었을시
  //연결 안되었을시
  if (Serial1.available()) {
    //    Serial.print(Serial1.available());
    Serial.write(Serial1.read());
    //Serial.print(Serial.write(Serial1.read()));
    //블루투스에 값이 있다면 블루투스가 읽은 것을 시리얼에 써라(보여줘라)
  }
  if (Serial.available()) {
    //  Serial.print(Serial.available());
    //    Serial.print(Serial1.write(Serial.read()));
    Serial1.write(Serial.read());
    //시리얼에 값이 있다면 시리얼이 읽은 것을 블루투스에 써라(보여줘라)
  }
}