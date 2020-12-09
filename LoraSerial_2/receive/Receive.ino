#include <SoftwareSerial.h>

#define RST_PIN 13
#define LORA_RX 2
#define LORA_TX 3
#define BUFFER_SIZE 30

const int setCmdDelay = 100; /*待機時間*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

void setup() {
  pinMode(RST_PIN, OUTPUT);
 // LoRaを再起動させる
  digitalWrite(RST_PIN, LOW);
  delay(100);
  digitalWrite(RST_PIN, HIGH); 
  delay(1500);

  Serial.begin(9600);
  LoraSerial.begin(115200);
  loraInit();
  delay(1500);
  Serial.begin("----------\n");
  Serial.begin("Read Start\n");
  Serial.begin("----------\n");
}

void loop() {
}

void loraInit() {
    Serial.print("Start...");
    // コマンドモード開始
    LoraSerial.println("2"); clearBuffer();
    //nodeの種別設定
    LoraSerial.println("node 2")
    // bw（帯域幅の設定）
    LoraSerial.println("bw 5"); clearBuffer();
    // sf（拡散率の設定）
    LoraSerial.println("sf 12"); clearBuffer();
    LoraSerial.println("channel 1"); clearBuffer();
    // 自分が参加するPANネットワークアドレスの設定
    LoraSerial.println("panid 0002"); clearBuffer();
    // 自分のノードIDを設定
    LoraSerial.println("ownid 1012"); clearBuffer();
    //送信元ノードネットワークアドレス
    LoraSerial.println("dstid ffff"); clearBuffer();
    // ack受信の設定
    LoraSerial.println("ack 2"); clearBuffer();
    // RRSIの付与設定
    LoraSerial.println("p 1"); clearBuffer();
    // UART転送速度設定
    LoraSerial.println("r 5"); clearBuffer();
    // 自動送信間隔
    LoraSerial.println("B 5"); clearBuffer();
    // 自動送信データ設定
    LoraSerial.println("C 1234567890"); clearBuffer();
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
    char Data[BUFFER_SIZE];
    while(LoraSerial.available > 0){
        Data[n] = LoraSerial.read();        //Dataを読み込む
        
        // 改行文字が来たら終端文字を挿入し、シリアルモニターで表示する
        if (Data[n] == '\r' || Data[n] == '\n')
        {
            Data[n] = '\0';
            clearBuffer();
            Serial.println(Data);
            break;
        }

        if (n < BUFFER_SIZE)//buffsizeを超えたら0に戻す
            n++;
        else
            n = 0;

        delay(300);
    }
}