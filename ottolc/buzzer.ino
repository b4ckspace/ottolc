// This file controls the buzzer for sound output

#define PIN_BUZZER 10

void initBuzzer(){
  pinMode(PIN_BUZZER,OUTPUT);
}

void ottoTone(int frequency, int duration){
  tone(PIN_BUZZER, frequency, duration);
}

void beep(){
  ottoTone(500, 200);
}

