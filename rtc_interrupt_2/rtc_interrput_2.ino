#include <Wire.h>
#define inPin 2
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

void interrput()
{
  Serial.println("interrupt_message");
}

void setup()
{
  pinMode(inPin, INPUT_PULLUP);         //2番をプルアップ設定
  attachInterrupt(0, interrput, FALLING); // 外部割り込みを開始する。
                                        // message 割り込み時に実行される関数
                                        // FALLING ピンの状態が HIGH → LOW になった時に割り込み
  Serial.begin(9600);                   //siralの速度
  Serial.print("start!!\n---------------------------\n");

  Wire.begin(); // arudinoをマスターとして接続
  delay(1000);  // 発振子の動作待機

  Wire.beginTransmission(RTCaddress); //接続するIC2のモジュールを選択
  Wire.write(0x00);                   // データを転送するレジスタ番号を指定
  Wire.write(0b00100000);                   // 00 Control 1　STOP(bit5)-1 をセットし動作を停止させる。
  Wire.write(0x00010001);                   // 01 Control 2 定周期タイマ割り込み設定（パルス出力のため5bit目を1）TIEを１に設定
  Wire.write(0b00000001);                   // 02 Seconds　
  Wire.write(0b00000000);                   // 03 Minutes
  Wire.write(0b00000000);                   // 04 Hours
  Wire.write(0b00000001);                   // 05 Days
  Wire.write(0b00000001);                   // 06 Weekdays
  Wire.write(0b00000001);                   // 07 Months
  Wire.write(0b00100000);                   // 08 Years
                                            //20年1月１日00:00に設定

  Wire.write(0x00); // 09 Minutes Alarm　
  Wire.write(0x00); // 0A Hours Alarm
  Wire.write(0x00); // 0B Days Alarm
  Wire.write(0x00); // 0C Weekdays Alarm

  Wire.write(0x00); // 0D CLKOUT 　　　　タイマー用レジスタ
  Wire.write(0x00); // 0E Timer control　　　　　〃
  Wire.write(0b00000101); // 0F Timer　　　　　　　　　〃
  Wire.write(0x00); // 00 Control 1　STOP(bit5)-0 をリセットし動作を開始する。
                    //    アドレス 0F の次は先頭アドレスの 00 に戻る。
  Wire.endTransmission();

  Wire.beginTransmission(RTCaddress); //接続するIC2のモジュールを選択
  Wire.write(0x0E);                         // データを転送する先頭のレジスタ番号を指定
  Wire.write(0b10000010);                   // 0E Timer control(1Hz = 1秒)
  Wire.endTransmission();
}

void loop()
{
  Wire.beginTransmission(RTCaddress);
  Wire.write(0x02);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(RTCaddress, 7);
  int sec = Wire.read();   // 秒
  int min = Wire.read();   // 分
  int hour = Wire.read();  // 時
  int day = Wire.read();   // 日
  int week = Wire.read();  // 曜日
  int month = Wire.read(); // 月
  int year = Wire.read();  // 年

  Serial.print(year, HEX);
  Serial.print("/");
  Serial.print(month & 0x1f, HEX);
  Serial.print("/");
  Serial.print(day & 0x3f, HEX);
  Serial.print("(");
  Serial.print(week & 0x07, HEX);
  Serial.print(")");
  Serial.print(hour & 0x3f, HEX);
  Serial.print(":");
  Serial.print(min & 0x7f, HEX);
  Serial.print(":");
  Serial.println(sec & 0x7f, HEX);

  delay(1000);
}