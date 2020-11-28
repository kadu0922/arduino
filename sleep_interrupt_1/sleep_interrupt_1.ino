#include <avr/sleep.h>
#define inPin 5
#define LED 13
void setup() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //スリープモード設定

  pinMode(LED, OUTPUT);//13を出力設定(LED用)
  pinMode(inPin, INPUT_PULLUP);//5番をプルアップ設定
  
  attachInterrupt(digitalPinToInterrupt(2), func_int, FALLING);//割り込み処理
}

void loop() {
  sleep_enable();     //スリープを有効化
  sleep_cpu();        //スリープ開始(ここでプログラムは停止する)
  sleep_disable();    //スリープを無効化
  digitalWrite(LED, 1);
  delay(5000);
  digitalWrite(LED, 0);
}

//割り込み処理ルーチン(空だけど必要)
void func_int() {
}
