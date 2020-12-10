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
  // bw（帯域幅の設定）
  LoraSerial.println("bw 5"); clearBuffer();
  // sf（拡散率の設定）
  LoraSerial.println("sf 12"); clearBuffer();
  LoraSerial.println("channel 1"); clearBuffer();
  // 自分が参加するPANネットワークアドレスの設定
  LoraSerial.println("panid 0002"); clearBuffer();
  // 自分のノードIDを設定
  LoraSerial.println("ownid 0012"); clearBuffer();
  // ack受信の設定
  LoraSerial.println("ack 2"); clearBuffer();
  //送信元のID付与設定
  LoraSerial.println("o 1"); clearBuffer();
  // RRSIの付与設定
  LoraSerial.println("p 1"); clearBuffer();
  // 送信モードを設定
  LoraSerial.println("n 2"); clearBuffer();
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