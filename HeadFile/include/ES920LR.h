#ifndef LORA_H　/*おまじない*/
#define LORA_H

#include "Arduino.h" /* arduino独自の定数を読み込む*/
Class Lora
{
public:
    void LoraSend(char config, char message);
    void SerialInit(int BautRate);

private:
    char config;
    char message;
}
#endif