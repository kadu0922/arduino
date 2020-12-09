#include <SoftwareSerial.h>

#define RST_PIN 13
#define LORA_RX 2
#define LORA_TX 3

const int setCmdDelay = 100; /*待機時間*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

void setup() {
    pinMode(RST_PIN, OUTPUT);
    // LoRaを再起動させる
    digitalWrite(RST_PIN, LOW);
    delay(100);
    digitalWrite(RST_PIN, HIGH); 
    delay(1500);

    Serial.begin(115200);
    LoraSerial.begin(115200);
    loraInit();
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
    LoraSerial.println("ownid 0012"); clearBuffer();
    //送信元ノードネットワークアドレス
    LoraSerial.println("dstid 1012"); clearBuffer();
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

// /*
//  a. node        select Coordinator or EndDevice
//  b. bw          select Band Width
//  c. sf          set Spreading Factor
//  d. channel     set channel
//  e. panid       set PAN ID
//  f. ownid       set Own Node ID
//  g. dstid       set Destination ID
//  l. ack         set Acknowledge Mode
//  m. retry       set send retry count
//  n. transmode   select Transfer Mode
//  o. rcvid       set received Node ID information
//  p. rssi        set RSSI information
//  q. operation   select Configuration or Operation
//  r. baudrate    select UART baudrate
//  s. sleep       select Sleep Mode
//  t. sleeptime   set Sleep Wakeup Timer value
//  u. power       set Output Power
//  v. version     software version
//  w. save        save parameters
//  x. load        load default parameters
//  y. show        show parameters
//  z. start       Transite Operation
//  A. format      set Data Format
//  B. sendtime    set test send interval
//  C. senddata    set test send data
//  ?. help        help
// */