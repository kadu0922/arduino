#include <Wire.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>

#define INPIN 2
#define LED 10
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

#define SLEEP_PIN 12    /* スリープピン */
#define RST_PIN 13      /* リセットピン */
#define LORA_RX 4       /* Software_RX_4 */
#define LORA_TX 5       /* Software_TX_5 */
#define CMDDELAY 100    /* CMD待機時間 */
#define BOOTDELAY 1500  /* Boot待機時間 */

#define BAUTRATE 9600   /* BautRate */

boolean RTC_FLAG = true; /* true = PACKET時間管理 false = sleep時間管理 */
boolean PACKET_FLAG = false; /* true = パケットキャプチャ成功　false = パケットキャプチャ失敗 */
boolean INIT_FLAG = true; /* true = 初回起動　false = 二回目以降*/
boolean WAIT_FLAG = false; /* true = 待機中　false =待機終了*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

/* rtcの動作停止用 */
void setResetRtc(){
    Wire.begin(); // arudinoをマスターとして接続
    delay(1000);  // 発振子の動作待機
    Wire.write(0x00);                   // データを転送するレジスタ番号を指定
    Wire.write(0b00100000);                   // 00 Control 1　STOP = 1 動作停止
}

/* Sleep用RTCの設定を初期化する関数 */
void setSleepRtcConfig(){
    RTC_FLAG = false;

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
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

void setPacketRtcConfig(){
    RTC_FLAG = true;

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
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

/* loraの初期化関数 */
void setLoraInit() {
    Serial.println("Start...");
    // コマンドモード開始
    setLoraConfig("2"); 
    //nodeの種別設定
    setLoraConfig("node 2");
    // bw（帯域幅の設定）
    setLoraConfig("bw 4"); 
    // sf（拡散率の設定）
    setLoraConfig("sf 12"); 
    //channel設定
    setLoraConfig("channel 1"); 
    // 自分が参加するPANネットワークアドレスの設定
    setLoraConfig("panid 0002"); 
    // 自分のノードIDを設定
    setLoraConfig("ownid 0002"); 
    //送信元ノードネットワークアドレス
    setLoraConfig("dstid 0003"); 
    // ack受信の設定
    setLoraConfig("ack 2"); 
    // 転送モード設定
    setLoraConfig("n 1"); 
    // RRSIの付与設定
    setLoraConfig("p 1"); 
    // sleepの設定
    setLoraConfig("sleep 3"); 
    // UART転送速度設定
    setLoraConfig("r 1"); 
    // 設定を保存する
    setLoraConfig("w");
    // 通信の開始
    setLoraConfig("z");
    // 送信データの内容
    Serial.println("Set up OK!");
}

/* loraにConfigを送る関数 */
void setLoraConfig(String Config){
    LoraSerial.println(Config);
    clearBuffer();
}

/* bufferを初期化する関数 */
void clearBuffer() {
    delay(CMDDELAY);
    while (LoraSerial.available() > 0) LoraSerial.read();
}

/* Loraを再起動させる関数 */
void setRestartLora(){
    pinMode(RST_PIN, OUTPUT);
    digitalWrite(RST_PIN, LOW);
    delay(BOOTDELAY);
    digitalWrite(RST_PIN, HIGH); 
    delay(BOOTDELAY);

    Serial.begin(BAUTRATE);
    LoraSerial.begin(BAUTRATE);
}

/* Arduino,Loraをスリープさせる関数 */
void setSystemSleep(){
    Serial.println("----SLLEP----Lora2-----");
    WAIT_FLAG = false;
    digitalWrite(LED, 0);                   //LED消灯
    digitalWrite(SLEEP_PIN, HIGH);          //Lora sleep_mode
    setSleepRtcConfig();                    //RTCをスリープように変更する
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    //スリープモード設定
    sleep_enable();     //スリープを有効化
    sleep_cpu();        //スリープ開始(ここでプログラムは停止する)
}

/* 割り込み関数 */
void interrput()
{
    if(RTC_FLAG){ 
        Serial.println("----WAIT----Lora2-----");
        WAIT_FLAG = true;
    }
}

/* LoraからDataを読み出してデータ部を送る関数*/
void setReadSendLoraData(){
    String Data;

    while(!PACKET_FLAG){
        delay(10);
        if (LoraSerial.read() != -1){
            PACKET_FLAG = true; //キャプチャ成功
            Data = LoraSerial.readStringUntil('\r');//ラインフィードまで格納する
            clearBuffer();
            Data = Data.substring(11);
            Serial.println(Data); //データ部分だけ表示シリアルモニターで表示
            LoraSerial.println(Data);  //Loraで送信する
            LoraSerial.flush();
            /* 初回起動時はINIT_FLAGをfalseにする*/
            if(INIT_FLAG) INIT_FLAG = false;
        }
        /*パケット待機時間が終了したとき抜ける*/
        if(WAIT_FLAG) break;
    }

}

void setSystemInit(){
    digitalWrite(LED, 1);
    //初回起動はパケットを受け取るまで待機

        setReadSendLoraData();
        if(!INIT_FLAG){
            setSystemSleep();
        }
}

/* Main関数 */
void setup()
{
    pinMode(SLEEP_PIN,OUTPUT);         //Loraのスリープピン初期化
    digitalWrite(SLEEP_PIN, LOW);      //Low = active_mode　High = sleep_mode

    setResetRtc();

    pinMode(LED, OUTPUT);                   //13を出力設定(LED用)
    
    pinMode(INPIN, INPUT_PULLUP);           //2番をプルアップ設定
    attachInterrupt(0, interrput, FALLING); // 外部割り込みを開始する。
                                            // message 割り込み時に実行される関数
                                            // FALLING ピンの状態が HIGH → LOW になった時に割り込み
    Serial.begin(9600);                     //siralの速度
    Serial.print("Lora2\n---------------------------\n");

    setRestartLora();
    setLoraInit();
    delay(1500);

    LoraSerial.readStringUntil(10); //OKの文字列を読み飛ばす

    setSystemInit();
}

void loop()
{
    sleep_disable();        //スリープを無効化

    Serial.println("----HELLO----Lora2-----");
    delay(100);
    digitalWrite(LED, 1);   //LED on
    digitalWrite(SLEEP_PIN, LOW);      //Low = active_mode　High = sleep_mode
    PACKET_FLAG = false;    //packetフラグ初期化
    setPacketRtcConfig();    //RTCをパケット待ち状態にする
    setReadSendLoraData();
    setSystemSleep();
}
