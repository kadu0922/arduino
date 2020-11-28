#include <Wire.h>
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

void message()
{
  Serial.println("INT");
}

void setup()
{
  attachInterrupt(0, message, FALLING); // 外部割り込みを開始する。
                                        // 0 割り込みピン D2
                                        // message 割り込み時に実行される関数
                                        // FALLING ピンの状態が HIGH → LOW になった時に割り込み
  Serial.begin(9600);                   //siralの速度
  Serial.print("startaaa!!\n---------------------------\n");

  Wire.begin(); // arudinoをマスターとして接続
  delay(1000);  // 発振子の動作待機

  Wire.beginTransmission(RTCaddress); //接続するIC2のモジュールを選択
  Wire.write(0x00);                   // データを転送する先頭のレジスタ番号を指定
  Wire.write(0x20);                   // 00 Control 1　STOP(bit5)-1 をセットし動作を停止させる。
  Wire.write(0x03);                   // 01 Control 2 定周期タイマ割り込み設定（パルス出力のため5bit目を1）TIEを１に設定
  Wire.write(0x00);                   // 02 Seconds　初期値を転送（秒）0 ～ 59
  Wire.write(0x00);                   // 03 Minutes　　　 〃 　　（分）0 ～ 59
  Wire.write(0x00);                   // 04 Hours　　　　 〃 　　（時）0 ～ 23
  Wire.write(0x01);                   // 05 Days 　　　　 〃 　　（日）1 ～ 31
  Wire.write(0x01);                   // 06 Weekdays 　　 〃 　　（曜日）0 ～ 6
  Wire.write(0x01);                   // 07 Months 　　　 〃 　　（月）1 ～ 12
  Wire.write(0x00);                   // 08 Years　　　　 〃 　　（年）0 ～ 99
                                      //20年1月１日00:00に設定

  Wire.write(0x00); // 09 Minutes Alarm　アラームの初期値を転送（分）0 ～ 59P
  Wire.write(0x00); // 0A Hours Alarm　　　　　　 〃 　　　　　（時）0 ～ 23
  Wire.write(0x00); // 0B Days Alarm 　　　　　　 〃 　　　　　（日）1 ～ 31
  Wire.write(0x00); // 0C Weekdays Alarm　　　　　〃 　　　　　（曜日）0 ～ 6

  Wire.write(0x00); // 0D CLKOUT 　　　　タイマー用レジスタ
  Wire.write(0x00); // 0E Timer control　　　　　〃
  Wire.write(0x00); // 0F Timer　　　　　　　　　〃
  Wire.write(0x00); // 00 Control 1　STOP(bit5)-0 をリセットし動作を開始する。
                    //    アドレス 0F の次は先頭アドレスの 00 に戻る。
  Wire.endTransmission();

  // Wire.beginTransmission(RTCaddress); //接続するIC2のモジュールを選択
  // Wire.write(0x0E);                         // データを転送する先頭のレジスタ番号を指定
  // Wire.write(0b00000000);                   // 0E Timer control　　　　　〃
  // Wire.endTransmission();

  // タイマーを停止
  Wire.beginTransmission(RTCaddress);
  Wire.write(0x0E);
  Wire.write(0b00000000); // 0E
  Wire.endTransmission();

  // Control 2 の設定
  Wire.beginTransmission(RTCaddress);
  Wire.write(0x01);
  Wire.write(0b00010001); // 02 bit4 TI/TP 動作モードを繰り返しに設定
                          //    bit0 TIE   /INT端子へ出力
  Wire.endTransmission();

  // タイマーのカウント数を設定
  Wire.beginTransmission(RTCaddress);
  Wire.write(0x0F);
  Wire.write(0b00000010); // 0F カウンタの回数を指定
  Wire.endTransmission();

  // タイマーを開始
  Wire.beginTransmission(RTCaddress);
  Wire.write(0x0E);
  Wire.write(0b10000010); // 0E bit7 TE   タイマー割り込み開始
                          //    bit1 TD1  1周期の間隔を設定 TD1,0 = 1,0 = 1 秒
                          //    bit0 TD0
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