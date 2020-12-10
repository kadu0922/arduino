#include <SoftwareSerial.h>

#define RST_PIN 13      /*リセットピン*/
#define LORA_RX 2       /*Software_RX_2*/
#define LORA_TX 3       /*Software_TX_3*/
#define CMDDELAY 100    /*待機時間*/
#define BOOTDELAY 1500  /*Boot待機時間*/
#define BAUTRATE 9600   /*BautRate*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

void setup() {
  pinMode(RST_PIN, OUTPUT);
 // LoRaを再起動させる
  digitalWrite(RST_PIN, LOW);
  delay(BOOTDELAY);
  digitalWrite(RST_PIN, HIGH); 
  delay(BOOTDELAY);

  Serial.begin(BAUTRATE);
  LoraSerial.begin(BAUTRATE);
  loraInit();
  delay(CMDDELAY);

  Serial.println("Read Start\n");
  Serial.println("----------\n");
}

void loop() {
  Serial.print("----------\n");
  LoraRead();
}

void loraInit() {
    Serial.println("Start...");
    // コマンドモード開始
    loraSend("2"); 
    //nodeの種別設定
    loraSend("node 2");
    // bw（帯域幅の設定）
    loraSend("bw 5"); 
    // sf（拡散率の設定）
    loraSend("sf 12"); 
    //channel設定
    loraSend("channel 1"); 
    // 自分が参加するPANネットワークアドレスの設定
    loraSend("panid 0002"); 
    // 自分のノードIDを設定
    loraSend("ownid 1012"); 
    //送信元ノードネットワークアドレス
    loraSend("dstid ffff"); 
    // ack受信の設定
    loraSend("ack 2"); 
    // 転送モード設定
    loraSend("n 1"); 
    // RRSIの付与設定
    loraSend("p 1"); 
    // UART転送速度設定
    loraSend("r 1"); 
    // 自動送信間隔
    loraSend("B 0"); 
    // 自動送信データ設定
    loraSend("C receive side"); 
    // 設定を保存する
    loraSend("w");
    // 通信の開始
    loraSend("z");
    Serial.println("Set up OK!");
}

void loraSend(String Config){
    LoraSerial.println(Config);
    clearBuffer();
}

void clearBuffer() {
  delay(CMDDELAY);
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