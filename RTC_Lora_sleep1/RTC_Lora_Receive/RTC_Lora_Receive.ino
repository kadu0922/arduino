#include <Wire.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>

#define inPin 2
#define LED 10
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

#define RST_PIN 13      /*リセットピン*/
#define LORA_RX 4       /*Software_RX_2*/
#define LORA_TX 5       /*Software_TX_3*/
#define CMDDELAY 100    /*CMD待機時間*/
#define BOOTDELAY 1500  /*Boot待機時間*/
#define BAUTRATE 9600   /*BautRate*/
#define READTIME 1000   /*読み込み時間*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

void interrput()
{
    Serial.println("interrupt_message");
    Serial.println("light up LED 5s");
}

void setRtcConfig(){
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

// LoRaを再起動させる関数
void restartLora(){
    pinMode(RST_PIN, OUTPUT);
    digitalWrite(RST_PIN, LOW);
    delay(BOOTDELAY);
    digitalWrite(RST_PIN, HIGH); 
    delay(BOOTDELAY);

    Serial.begin(BAUTRATE);
    LoraSerial.begin(BAUTRATE);
}

void loraInit() {
    Serial.println("Start...");
    // コマンドモード開始
    loraConfigSend("2"); 
    //nodeの種別設定
    loraConfigSend("node 2");
    // bw（帯域幅の設定）
    loraConfigSend("bw 5"); 
    // sf（拡散率の設定）
    loraConfigSend("sf 12"); 
    //channel設定
    loraConfigSend("channel 1"); 
    // 自分が参加するPANネットワークアドレスの設定
    loraConfigSend("panid 0002"); 
    // 自分のノードIDを設定
    loraConfigSend("ownid 1012"); 
    //送信元ノードネットワークアドレス
    loraConfigSend("dstid ffff"); 
    // ack受信の設定
    loraConfigSend("ack 2"); 
    // 転送モード設定
    loraConfigSend("n 1"); 
    // RRSIの付与設定
    loraConfigSend("p 1"); 
    // UART転送速度設定
    loraConfigSend("r 1"); 
    // 自動送信間隔
    loraConfigSend("B 0"); 
    // 自動送信データ設定
    loraConfigSend("C receiveSide"); 
    // 設定を保存する
    loraConfigSend("w");
    // 通信の開始
    loraConfigSend("z");
    Serial.println("Set up OK!");
}

void loraConfigSend(String Config){
    LoraSerial.println(Config);
    clearBuffer();
}

void clearBuffer() {
    delay(CMDDELAY);
    while (LoraSerial.available() > 0) LoraSerial.read();
}

void loraDataRead(){
    String Data;
    if(LoraSerial.read() == -1){
        Serial.println("Nothing Data");
    }else{
        Data = LoraSerial.readStringUntil('\r\n');//CRおよびLFのため
        clearBuffer();
        Serial.println(Data.substring(11)); //データ部分だけ表示
    }
    delay(READTIME);
}

void setup()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //スリープモード設定

    pinMode(LED, OUTPUT);                   //13を出力設定(LED用)
    pinMode(inPin, INPUT_PULLUP);           //5番をプルアップ設定

    attachInterrupt(0, interrput, FALLING); // 外部割り込みを開始する。
                                            // message 割り込み時に実行される関数
                                            // FALLING ピンの状態が HIGH → LOW になった時に割り込み

    Serial.begin(9600);                     //siralの速度
    Serial.print("start!!\n---------------------------\n");

    restartLora();
    loraInit();
    setRtcConfig();
}

void loop()
{
    int n = 0;
    sleep_enable();     //スリープを有効化
    sleep_cpu();        //スリープ開始(ここでプログラムは停止する)
    sleep_disable();    //スリープを無効化
    digitalWrite(LED, 1);
    while (n < 5)
    {
        loraDataRead();
        n++;
    }
    
    n = 0;
    
    digitalWrite(LED, 0);
}