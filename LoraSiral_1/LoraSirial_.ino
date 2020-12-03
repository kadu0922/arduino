void setup()
{
    Serial.begin(9600); //シリアル通信の速度
    deley(3000);
    LoraInit();
}

void loop()
{
}

void LoraInit()
{
    Serial.println("1"); //configmode

    Serial.println("a"); //ノード種別
    Serial.println("2"); /*  1. Coordinator 2. EndDevice */

    Serial.println("b"); //帯域幅
    Serial.println("5"); /*  3. 62.5kHz
                                4. 125kHz
                                5. 250kHz
                                6. 500kHz */

    Serial.println("c");  //拡散率
    Serial.println("12"); //拡散率:12

    Serial.println("d"); //チャンネル
    Serial.println("1"); //チャンネル:1

    Serial.println("e");    //PanId
    Serial.println("0002"); //PanId:0002

    Serial.println("f"); //OwnId
    Serial.println("0012");

    Serial.println("g"); //DestId
    Serial.println("1012");

    Serial.println("l"); //Ack
    Serial.println("2"); /* 1. ON 2. OFF*/

    Serial.println("p"); //Rssi
    Serial.println("1"); /* 1. ON 2. OFF*/

    Serial.println("B"); //sendtime
    Serial.println("5"); //X秒

    Serial.println("C"); //senddata
    Serial.println("1234567890");

    Serial.println("w"); //save

    Serial.println("z"); //start
    Serial.print("finish!!!\n");
}
/*
 a. node        select Coordinator or EndDevice
 b. bw          select Band Width
 c. sf          set Spreading Factor
 d. channel     set channel
 e. panid       set PAN ID
 f. ownid       set Own Node ID
 g. dstid       set Destination ID
 l. ack         set Acknowledge Mode
 m. retry       set send retry count
 n. transmode   select Transfer Mode
 o. rcvid       set received Node ID information
 p. rssi        set RSSI information
 q. operation   select Configuration or Operation
 r. baudrate    select UART baudrate
 s. sleep       select Sleep Mode
 t. sleeptime   set Sleep Wakeup Timer value
 u. power       set Output Power
 v. version     software version
 w. save        save parameters
 x. load        load default parameters
 y. show        show parameters
 z. start       Transite Operation
 A. format      set Data Format
 B. sendtime    set test send interval
 C. senddata    set test send data
 ?. help        help
*/