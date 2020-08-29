#include <SPI.h>

void setup() {
  Serial.begin(9600);
  pinMode( SS, OUTPUT ); //デジタル10番ピンをOUTPUTに設定
  digitalWrite( SS, HIGH ); //デジタルの10番ピンからHighを出力

  SPI.setBitOrder( LSBFIRST ); // 下位ビットから送信
  SPI.setClockDivider( SPI_CLOCK_DIV8 ); //分周器1/8を指定 同期クロック16/8=2MHz
  SPI.setDataMode( SPI_MODE0 ); //アイドル時Low、立上がりエッジでデータ取得

  SPI.begin(); //SPI通信の初期化、有効化
}


void loop() {
  int value=1;
  digitalWrite( SS, LOW ); //対象の電子部品を接続したSS
  while(true){
  value = SPI.transfer(value); //valueに電子部品から受信したデータを格納
  Serial.print("-- ");
  Serial.print(value);
  Serial.print(" --\n");
  delay( 100 ); //必要に応じて設定
  value++;
  }
  digitalWrite( SS, HIGH ); //通信
}
