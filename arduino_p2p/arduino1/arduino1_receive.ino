int LED = 13;

void setup(){
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop(){
  while(Serial.available()){
    byte inChar = (byte)Serial.read();
    if((inChar % 2) == 1){ 
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }
    delay(10);
  }
}