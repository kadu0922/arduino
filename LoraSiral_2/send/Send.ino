#include <ES920LR.h>

void setup()
{
    Lora SerialInit(115200);
    deley(3000);
    LoraInit();
}

void loop()
{
}

void LoraInit()
{
    Lora.LoraSend("1"); //configmode

    Lora.LoraSend("a", "2"); //ノード種別
                             /*  1. Coordinator 2. EndDevice */

    Lora.LoraSend("b", "5"); //帯域幅
                             /*  3. 62.5kHz
                                4. 125kHz
                                5. 250kHz
                                6. 500kHz */

    Lora.LoraSend("c", "12"); //拡散率:12

    Lora.LoraSend("d", "1"); //チャンネル:1

    Lora.LoraSend("e", "0002"); //PanId:0002

    Lora.LoraSend("f", "0012"); //OwnId:0012

    Lora.LoraSend("g", "1012"); //DestId:1012

    Lora.LoraSend("l", "2"); // Ack: 1. ON 2. OFF

    Lora.LoraSend("p", "1"); // RSSi: 1. ON 2. OFF

    Lora.LoraSend("b", "5"); //sendtime:X秒

    Lora.LoraSend("c", "1234567890"); //senddata

    Lora.LoraSend("w"); //save

    Lora.LoraSend("z"); //start
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