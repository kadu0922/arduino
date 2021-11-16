#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

int LED = 13;
int count = 0;

void setup(){
  Serial.begin(115200);  
  mySerial.begin(115200); // ソフトウェアシリアルの初期化

  pinMode(LED, OUTPUT);
}

void loop(){
  count++;
  Serial.print("counter = ");
  Serial.println(count); //通常のシリアル通信
  
  mySerial.listen(); //SoftwareSerialに切り替え
  mySerial.write(count); //SoftwareSerial使用
  
  if((count % 2) == 1){ 
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
  delay(1000);
}