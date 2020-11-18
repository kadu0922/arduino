#include <Wire.h>


int RTCDATA[16];  //read buf. from RTC unit

#define RTCaddress 0xa2 >> 1
//TWI address of RTC-unit

#define inPin 8
//set digital8 as input


void setup() {

  pinMode(inPin, INPUT);
  digitalWrite(inPin, HIGH);   //pull up


  Serial.begin(9600);

  Serial.println("initializing RTC unit");
  //init message

  delay(1000);

  Wire.begin();  // join the TWI as bus-master

  Wire.beginTransmission(RTCaddress);
  //write to RTC address (write mode)
  Wire.write(0x00);
  //set internal register address
  // of RTC as 0 (auto increment)

  //set initial time & date & year
  // as 2008.12.25  12:20:15
  Wire.write(0x00);
  //control1 = 0x00
  Wire.write(0x03);
  //control2 = 0x03
  Wire.write(0x15);
  //seconds = 0x15
  Wire.write(0x20);
  //minutes = 0x20
  Wire.write(0x12);
  //hours = 0x12
  Wire.write(0x25);
  //days = 0x25
  Wire.write(0x04);
  //weekdays = 0x04 (Thursday)
  Wire.write(0x92);
  //month/century = 0x92 (0x80 | 0x12)
  Wire.write(0x08);
  //years = 0x08

  Wire.write(0x21);
  //minutes alarm = 0x21
  Wire.write(0x12);
  //hours alarm = 0x12
  Wire.write(0x25 | 0x80);
  //days alarm = 0x25
  Wire.write(0x04 | 0x80);
  //weekdays alarm = 0x04 (Thursday)

  Wire.write(0b10000011);
  //clk out = 0x10000011 (1hz)

  Wire.endTransmission();
  delay(10);

  Serial.println("finished initializing RTC unit");
  //init message

}


void loop(void) {

  int i;

  Wire.beginTransmission(RTCaddress);
  //write to RTC address with write mode
  Wire.write(0x00);
  //set internal register address of RTC as 0
  Wire.endTransmission();
  //
  Wire.requestFrom(RTCaddress, 16);
  //request 16byte data from RTC
  //reading loop
  for (i = 0; i < 16; i++)
  {
    while (Wire.available() == 0 ) {
    }
    RTCDATA[i] = Wire.read();
  }

  //read data from RTC unit
  Serial.print(RTCDATA[8], HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[7] & 0x1f, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[5] & 0x3f, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[6] & 0x07, HEX);
  Serial.print("     ");

  Serial.print(RTCDATA[4] & 0x3f, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[3] & 0x7f, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[2] & 0x7f, HEX);
  Serial.print("     ");

  Serial.print(RTCDATA[11] & 0x3f, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[12] & 0x07, HEX);
  Serial.print(" ");
  Serial.print(RTCDATA[10] & 0x3f, HEX);
  Serial.print(" ");
  Serial.println(RTCDATA[9] & 0x7f, HEX);


  if (digitalRead(inPin)  == LOW) {
    Wire.beginTransmission(RTCaddress);
    //write to RTC address (write mode)
    Wire.write(0x01);
    //set internal register address of RTC as 1
    Wire.write(0x03);
    //clear control2 as 0x03
    Wire.endTransmission();
  }

  delay(100);
}
