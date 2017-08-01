// This file controls the buzzer for sound output
/*
#define PIN_BUZZER 10

void initBuzzer(){
  pinMode(PIN_BUZZER,OUTPUT);
}

void ottoTone(int frequency, int duration){
   //tone(PIN_BUZZER, frequency, duration);
}

void beep(){
  ottoTone(500, 200);
}
*/

//#include <NewTone.h>

//#define TONE_PIN 2 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).

// Melody (liberated from the toneMelody Arduino example sketch by Tom Igoe).
//int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
//int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };



void beep(){
  //ottoTone(500, 200);
  Serial.println("i wont beep");
}
//######################################################################


#define PIN_BUZZER 10
void initBuzzer(){
  pinMode(PIN_BUZZER,OUTPUT);
}

// Some Variables
int tone_ = 1000 * 1;
unsigned long toneChange = 0;
unsigned long toneBegin = 0;
long toneDuration = 1000 * 1000 * 3;
bool toneState = false;


void softTone(){
  unsigned long now = micros();
  if ( now < toneBegin + toneDuration ){
    //Serial.println("Tone START");
    if ( now - toneChange >= tone_ / 2 ){
      if ( toneState == false){
        digitalWrite(PIN_BUZZER,HIGH);
        toneState = true;
      }else{
        digitalWrite(PIN_BUZZER,LOW);
        toneState = false;
      }
      toneChange = now;
    }
    
  }else{
    toneBegin = now;
    Serial.println("Tone END");
  }
}
























