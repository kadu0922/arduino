#include <Wire.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>

#define INPIN 2
#define LED 10
#define RTCaddress 0xa2 >> 1
//RTC8564のスレーブアドレスは『0xA2』固定だが、Wireライブラリでは7ビットでデバイス特定をするため、右に1ビットシフトさせて指定

#define MAXTIME 7       /* sleep時間を合わせる定義*/
#define SLEEP_PIN 12    /* スリープピン */
#define RST_PIN 13      /* リセットピン */
#define LORA_RX 4       /* Software_RX_4 */
#define LORA_TX 5       /* Software_TX_5 */
#define CMDDELAY 100    /* CMD待機時間 */
#define BOOTDELAY 1500  /* Boot待機時間 */
#define READTIME 2000   /* 読み込み時間 */
#define SENDTIME 1000   /* 送信時間 */

#define BAUTRATE 9600   /* BautRate */

boolean SLEEP_FLAG = false; /* true = active false = sleep */
boolean PACKET_FLAG = false; /* true = パケットキャプチャ成功　false = パケットキャプチャ失敗 */
boolean INIT_FLAG = true; /* true = 初回起動　false = 二回目以降*/
SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

/* RTCの設定を初期化する関数 */
void setSleepRtcConfig(){
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
    Wire.write(0b00001000); // 0F Timer 8秒設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

void setPacketRtcConfig(){
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
    Wire.write(0b00000111); // 0F Timer 7秒設定

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
    setLoraConfig("bw 5"); 
    // sf（拡散率の設定）
    setLoraConfig("sf 12"); 
    //channel設定
    setLoraConfig("channel 1"); 
    // 自分が参加するPANネットワークアドレスの設定
    setLoraConfig("panid 0002"); 
    // 自分のノードIDを設定
    setLoraConfig("ownid 0003"); 
    //送信元ノードネットワークアドレス
    setLoraConfig("dstid 0004"); 
    // ack受信の設定
    setLoraConfig("ack 2"); 
    // 転送モード設定
    setLoraConfig("n 1"); 
    // RRSIの付与設定
    setLoraConfig("p 1"); 
    // sleepの設定
    setLoraConfig("s 1"); 
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
/* Arduino,Loraをスリープさせる関数 */
void setSystemSleep(){
    SLEEP_FLAG = true;
    digitalWrite(LED, 0);                   //LED消灯
    digitalWrite(SLEEP_PIN, HIGH);          //Lora sleep_mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    //スリープモード設定
    sleep_enable();     //スリープを有効化
    sleep_cpu();        //スリープ開始(ここでプログラムは停止する)
}

/* Sleepを解除する割り込み関数 */
void interrput()
{
        Serial.println("Relay1 Lora");
        SLEEP_FLAG = false; //sleepフラグ初期化
        PACKET_FLAG = false; //packetフラグ初期化
        sleep_disable();    //スリープを無効化
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
    }
}

/* Main関数 */
void setup()
{
    pinMode(SLEEP_PIN,OUTPUT);         //Loraのスリープピン初期化
    digitalWrite(SLEEP_PIN, LOW);      //Low = active_mode　High = sleep_mode

    pinMode(LED, OUTPUT);                   //13を出力設定(LED用)
    pinMode(INPIN, INPUT_PULLUP);           //2番をプルアップ設定
    attachInterrupt(0, interrput, FALLING); // 外部割り込みを開始する。
                                            // message 割り込み時に実行される関数
                                            // FALLING ピンの状態が HIGH → LOW になった時に割り込み
    Serial.begin(9600);                     //siralの速度
    Serial.print("start!!\n---------------------------\n");

    setRestartLora();
    setLoraInit();
    delay(1500);

    LoraSerial.readStringUntil(10); //OKの文字列を読み飛ばす
    digitalWrite(LED, 1);
}

void loop()
{
    //初回起動はパケットを受け取るまで待機
    while (INIT_FLAG)
    {
        setReadSendLoraData();
        if(!INIT_FLAG){
            setSleepRtcConfig();
            setSystemSleep();
        }
    }
}