#include <SoftwareSerial.h>

#define ES920LR_RST_PIN 13
#define LORA_RX 2
#define LORA_TX 3

const int setCmdDelay = 100; /*待機時間*/

SoftwareSerial LoRa_Serial(LORA_RX, LORA_TX);

void setup() {
  pinMode(ES920LR_RST_PIN, OUTPUT);
 // LoRaを再起動させる
  digitalWrite(ES920LR_RST_PIN, LOW);
  delay(100);
  digitalWrite(ES920LR_RST_PIN, HIGH); 
  delay(1500);

  Serial.begin(9600);
  LoRa_Serial.begin(9600);
  loraInit();
  
}

void loop() {
}

void loraInit() {
  Serial.print("Start...");
  // コマンドモード開始
  LoRa_Serial.println("2"); clearBuffer();
  // bw（帯域幅の設定）
  LoRa_Serial.println("bw 5"); clearBuffer();
  // sf（拡散率の設定）
  LoRa_Serial.println("sf 12"); clearBuffer();
  LoRa_Serial.println("channel 1"); clearBuffer();
  // 自分が参加するPANネットワークアドレスの設定
  LoRa_Serial.println("panid 0002"); clearBuffer();
  // 自分のノードIDを設定
  LoRa_Serial.println("ownid 0012"); clearBuffer();
  // ack受信の設定
  LoRa_Serial.println("ack 2"); clearBuffer();
  //送信元のID付与設定
  LoRa_Serial.println("o 1"); clearBuffer();
  // RRSIの付与設定
  LoRa_Serial.println("p 1"); clearBuffer();
  // 送信モードを設定
  LoRa_Serial.println("n 2"); clearBuffer();
  // 設定を保存する
  LoRa_Serial.println("w"); clearBuffer();
  // 通信の開始
  LoRa_Serial.println("z"); clearBuffer();
  Serial.println("Set up OK!");
}

void clearBuffer() {
  delay(setCmdDelay);
  while (LoRa_Serial.available() > 0) LoRa_Serial.read();
}

//void setup(){
 // Serial.begin(9600); //シリアル通信の速度

//}

//void loop(){
  //LoraInit();
//}

//void LoraInit(){
  //Serial.println("1\r\n");      /*configmode*/
  //Serial.flush();
  //Serial.println("a\r\n");      /*ノード種別*/
  //Serial.flush();
  //Serial.println("2\r\n");      /*  1. Coordinator 2. EndDevice */
  //Serial.flush();
  //Serial.println("b\r\n");/    /*帯域幅*/
  //Serial.flush();
  //Serial.println("5\r\n");      /*  3. 62.5kHz*/
                                 /*4. 125kHz*/
                                 /*5. 250kHz*/
                                 /*6. 500kHz*/
  //Serial.flush();
  //Serial.println("c\r\n");      /*拡散率*/
  //Serial.flush();
  //Serial.println("12\r\n");     /*拡散率:12*/
  //Serial.flush();
  //Serial.println("d\r\n");      /*チャンネル*/
  //Serial.flush();
  //Serial.println("1\r\n");      /*チャンネル:1*/
  //Serial.flush();
  //Serial.println("e\r\n");      /*PanId*/
  //Serial.flush();
  //Serial.println("0002\r\n");   /*PanId:0002*/
  //Serial.flush();
  //Serial.println("f\r\n");      /*OwnId*/
  //Serial.flush();
  //Serial.println("0012\r\n");
  //Serial.println("g\r\n");      /*DestId*/
  //Serial.flush();
  //Serial.println("1012\r\n");
  //Serial.flush();
  //Serial.println("l\r\n");      /*Ack*/
  //Serial.flush();
  //Serial.println("2\r\n");      /* 1. ON 2. OFF*/
  //Serial.flush();
  //Serial.println("p\r\n");      /*Rssi*/
  //Serial.flush();
  //Serial.println("1\r\n");      /* 1. ON 2. OFF*/
  //Serial.flush();
  //Serial.println("B\r\n");     /*sendtime*/
  //Serial.flush();
  //Serial.println("5\r\n");     /*X秒*/
  //Serial.flush();
  //Serial.println("C\r\n");     /*senddata*/
  //Serial.flush();
  //Serial.println("1234567890\r\n");    
  //Serial.flush();
  //Serial.println("w\r\n");      /*save*/
  //Serial.flush();
  //Serial.println("z\r\n");      /*start*/
  //Serial.flush();
  //Serial.print("finish!!!\n");
// }
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