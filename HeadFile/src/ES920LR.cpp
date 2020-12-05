#include "Arduino.h"
#include "ES920LR.h"

Lora::SerialInit(int BautRate)
{
    _BautRate = BaudRate;
}

Lora::LoraSend(char config, char message)
{
    Serial.begin(_BautRate);
    Serial.println(config);
    Serial.flush();

    if (message != NULL)
    {
        Serial.println(message);
        Serial.flush();
    }
}