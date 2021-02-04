#include <Wire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <EEPROM.h>
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
#define BOOTDELAY 1000  /* Boot待機時間 */

#define BAUTRATE 9600   /* BautRate */

boolean RTC_FLAG = false;          /* true = PACKET時間管理 false = sleep時間管理 */
boolean TIMEOUT_FLAG = false;     /* true = タイムアウトした false = タイムアウトしてない */
boolean PACKET_FLAG = false;      /* true = パケットキャプチャ成功　false = パケットキャプチャ失敗 */
boolean SLEEP_FLAG = false;        /*true = 分を計測　false = 秒を計測*/
boolean WAIT_FLAG = false;        /* true = 待機中　false =待機終了*/

SoftwareSerial LoraSerial(LORA_RX, LORA_TX);

//Arduinoをリセットする
void software_reset() {
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
}

byte Load_Bootstate() {
    int state;
    state = EEPROM.read(0);
    return state;
}

void Set_Bootstate(int state) {
    EEPROM.write(0, state);
}

/* RTCが分計測するためのSleep用関数 */
void setSleepRtcConfig(){
    RTC_FLAG = false;
    SLEEP_FLAG = true; 

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
    Wire.write(0b10000011); // 0E TimerControl
    Wire.write(0b00001111); // 0F Timer 14分設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

/* RTCがタイムアウト時間を計測するための関数 */
void setPacketRtcConfig(){
    RTC_FLAG = true;

    Wire.begin(); // arudinoをマスターとして接続

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
    Wire.write(0b000001000); // 0F Timer 8秒設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

/* RTCが秒を計測するためのSleep用関数 */
void setRtcTrue(){
    RTC_FLAG = false;
    SLEEP_FLAG = false;

    Wire.begin(); // arudinoをマスターとして接続

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
    Wire.write(0b00111001); // 0F Timer 57秒設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

/* RTCがタイムアウトしたときの秒を計測するためのSleep用関数 */
void setRtcFalse(){
    RTC_FLAG = false;
    SLEEP_FLAG = false;

    Wire.begin(); // arudinoをマスターとして接続

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
    Wire.write(0b00110110); // 0F Timer 54秒設定

    // Control 設定
    Wire.write(0x00);       // 00 Control 1　STOP = 0 動作開始
    Wire.write(0b00000001); //Control 2 Ti/Tp = 0 TIE = 1
    Wire.endTransmission();
}

/* loraの初期化関数 */
void setLoraInit() {

    // コマンドモード開始
    setLoraConfig("2");
    //channel設定
    setLoraConfig("channel 1"); 
    //nodeの種別設定
    setLoraConfig("node 2");
    // bw（帯域幅の設定）
    setLoraConfig("bw 4"); 
    // sf（拡散率の設定）
    setLoraConfig("sf 12"); 
    // 自分が参加するPANネットワークアドレスの設定
    setLoraConfig("panid 0002"); 
    // 自分のノードIDを設定
    setLoraConfig("ownid 0004"); 
    //送信元ノードネットワークアドレス
    setLoraConfig("dstid fffc"); 
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
    Serial.println("Lora4 Ready\n---------------------------");
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

    LoraSerial.begin(BAUTRATE);
}

/* Arduino,Loraをスリープさせる関数 */
void setSystemSleep(){
    Serial.println("----SLLEP----Lora4-----");
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
        Serial.println("----TimeOut-----");
        WAIT_FLAG = true;
        TIMEOUT_FLAG = true;
    }else{
        if(!SLEEP_FLAG){
            if(TIMEOUT_FLAG)    
                setRtcTrue();
            else
                setRtcFalse();
        }
    }
}

/* LoraからDataを読み出してデータ部を送る関数*/
void setReadSendLoraData(){
    String Data;
    TIMEOUT_FLAG = false;
    String List[5] = {"\0"}; // 分割された文字列を格納する配列 
    while(!PACKET_FLAG){
        delay(10);
        if (LoraSerial.read() != -1){
            PACKET_FLAG = true; //キャプチャ成功
            Data = LoraSerial.readStringUntil('\r');//ラインフィードまで格納する
            clearBuffer();
            Data = Data.substring(3);
            
            int index = split(Data, '&', List);
            for(int i = 0; i < index; i++){
                Serial.println(List[i]);
            }

            Serial.println(Data); //データ部分だけ表示シリアルモニターで表示
            LoraSerial.println(Data);  //Loraで送信する
            LoraSerial.flush();

        }
        /*パケット待機時間が終了したとき抜ける*/
        if(WAIT_FLAG) break;
    }

}

void setSystemInit(){
    digitalWrite(LED, 1);
    //初回起動はパケットを受け取るまで待機

    setReadSendLoraData();
    setSystemSleep();
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

    setRestartLora();
    delay(BOOTDELAY);
    if (Load_Bootstate() == 0) { //メモリに0が書いてあるので再起動する
        Set_Bootstate(1); //EEPROMに1を書いておく　次は通常起動
        delay(BOOTDELAY);
        software_reset();
    }
    Set_Bootstate(0); //メモリに0を書いておく。

    
    
    setLoraInit();
    delay(CMDDELAY);

    LoraSerial.readStringUntil(10); //OKの文字列を読み飛ばす
    LoraSerial.flush();

    setSystemInit();
}

void loop()
{
    sleep_disable();        //スリープを無効化
    
    Serial.println("----HELLO----Lora4-----");
    delay(100);
    digitalWrite(LED, 1);   //LED on
    digitalWrite(SLEEP_PIN, LOW);      //Low = active_mode　High = sleep_mode
    PACKET_FLAG = false;    //packetフラグ初期化
    setPacketRtcConfig();    //RTCをパケット待ち状態にする
    setReadSendLoraData();
    setSystemSleep();
}

int split(String data, char delimiter, String *dst){
    int index = 0;
    int arraySize = (sizeof(data)/sizeof((data)[0]));  
    int datalength = data.length();
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            if ( index > (arraySize - 1)) return -1;
        }
        else dst[index] += tmp;
    }
    return (index + 1);
}