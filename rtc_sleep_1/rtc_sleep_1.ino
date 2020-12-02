#include <Wire.h>
#include <avr/sleep.h>

#define inPin 2
#define LED 13
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

void interrput()
{
    Serial.println("interrupt_message");
    Serial.println("light up LED 5s");
}

void setup()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //スリープモード設定

    pinMode(LED, OUTPUT);                   //13を出力設定(LED用)
    pinMode(inPin, INPUT_PULLUP);           //2番をプルアップ設定
    attachInterrupt(0, interrput, FALLING); // 外部割り込みを開始する。
                                            // message 割り込み時に実行される関数
                                            // FALLING ピンの状態が HIGH → LOW になった時に割り込み
    Serial.begin(9600);                     //siralの速度
    Serial.print("start!!\n---------------------------\n");

    Wire.begin(); // arudinoをマスターとして接続
    delay(1000);  // 発振子の動作待機

    Wire.beginTransmission(RTCaddress); //接続するIC2のモジュールを選択
    Wire.write(0x00);                   // データを転送するレジスタ番号を指定
    Wire.write(0b00100000);                   // 00 Control 1　STOP = 1 動作停止
    Wire.write(0x00);                         // 01 Control 2 
    Wire.write(0b00000001);                   // 02 Seconds　
    Wire.write(0b00000000);                   // 03 Minutes
    Wire.write(0b00000000);                   // 04 Hours
    Wire.write(0b00000001);                   // 05 Days
    Wire.write(0b00000001);                   // 06 Weekdays
    Wire.write(0b00000001);                   // 07 Months
    Wire.write(0b00100000);                   // 08 Years
    //20年1月１日00:00に設定

    //Alram レジスタ
    Wire.write(0x00);       // 09 Minutes Alarm　
    Wire.write(0x00);       // 0A Hours Alarm
    Wire.write(0x00);       // 0B Days Alarm
    Wire.write(0x00);       // 0C Weekdays Alarm

    //timerレジスタ
    Wire.write(0x00);       // 0D CLKOUT
    Wire.write(0b10000010); // 0E TimerControl
    Wire.write(0b00001111); // 0F Timer 15秒設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00010001); //Control 2 Ti/Tp = 1 TIE = 1
    Wire.endTransmission();
}

void loop()
{
    sleep_enable();     //スリープを有効化
    sleep_cpu();        //スリープ開始(ここでプログラムは停止する)
    sleep_disable();    //スリープを無効化
    digitalWrite(LED, 1);
    delay(5000);
    digitalWrite(LED, 0);
}