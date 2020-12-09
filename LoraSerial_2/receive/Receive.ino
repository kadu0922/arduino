#include <SoftwareSerial.h>

#define RST_PIN 13
#define LORA_RX 2
#define LORA_TX 3
#define BUFFER_SIZE 500

const int setCmdDelay = 100; /*待機時間*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

void setup() {
  pinMode(RST_PIN, OUTPUT);
 // LoRaを再起動させる
  digitalWrite(RST_PIN, LOW);
  delay(100);
  digitalWrite(RST_PIN, HIGH); 
  delay(3500);

  Serial.begin(9600);
  LoraSerial.begin(9600);
  loraInit();
  delay(1500);

  Serial.println("Read Start\n");
  Serial.println("----------\n");
}

void loop() {
  Serial.print("----------\n");
  LoraRead();
}

void loraInit() {
    Serial.print("Start...");
    // コマンドモード開始
    LoraSerial.println("2"); clearBuffer();
    //nodeの種別設定
    LoraSerial.println("node 2");
    //トランスモードをpayloadにする
    LoraSerial.println("n 1");
    // bw（帯域幅の設定）
    LoraSerial.println("bw 5"); clearBuffer();
    // sf（拡散率の設定）
    LoraSerial.println("sf 12"); clearBuffer();
    //チャンネル1
    LoraSerial.println("channel 1"); clearBuffer();
    // 自分が参加するPANネットワークアドレスの設定
    LoraSerial.println("panid 0002"); clearBuffer();
    // 自分のノードIDを設定
    LoraSerial.println("ownid 1012"); clearBuffer();
    //送信元ノードネットワークアドレス
    LoraSerial.println("dstid ffff"); clearBuffer();
    // ack受信の設定
    LoraSerial.println("ack 2"); clearBuffer();
    // 送信元jのID設定
    LoraSerial.println("o 1"); clearBuffer();
    // RRSIの付与設定
    LoraSerial.println("p 1"); clearBuffer();
    // UART転送速度設定
    LoraSerial.println("r 1"); clearBuffer();
    // 設定を保存する
    LoraSerial.println("w"); clearBuffer();
    // 通信の開始
    LoraSerial.println("z"); clearBuffer();
    Serial.println("Set up OK!");

}

void clearBuffer() {
  delay(setCmdDelay);
  while (LoraSerial.available() > 0) LoraSerial.read();
}

void LoraRead(){
    int n = 0;      //添字
    String Data;

    if(LoraSerial.read() == -1){
          Serial.println("Nothing Data");
          delay(1000);
    }else{
      
      Data = LoraSerial.readStringUntil('\r\n');//CRおよびLFのため
      
      clearBuffer();
      Serial.println(Data);
      delay(1000);
    
  }
}