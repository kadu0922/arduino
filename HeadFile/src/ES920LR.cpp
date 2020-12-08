#include "Arduino.h"
#include "ES920LR.h"
#define BUFFER_SIZE 30

Lora::SerialInit(int BautRate)
{
    _BautRate = BaudRate;
}

Lora::LoraSend(char config, char message)
{
    Serial.begin(_BautRate);

    Serial.println(config); /* configを書き込む　完了まで待機*/
    Serial.flush();

    if (message != NULL) /* 付属のコンテンツがある場合 */
    {
        Serial.println(message);
        Serial.flush();
    }
}

Lora::LoraRead()
{
    int n = 0;
    char Data[BUFFER_SIZE] Serial.begin(_BautRate);

    While(Serial.available() > 0)
    {
        Data[n] = Serial.read();

        // 改行文字が来たらNULL文字にする
        if (Data[n] == '\r' || Data[n] == '\n')
        {
            Data[n] = '\0';
            clearBuffer();
            Serial.println(Data);
            break;
        }

        if (n < RECV_SIZE)
            n++;
        else
            n = 0;

        delay(300);
    }
}