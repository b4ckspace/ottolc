// this file manages general movement and servo comms
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>


#define N_SERVOS 4


#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2

Oscillator servo[N_SERVOS];


void initMovement(){
  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);
  resetServos();
}

void resetServos(){
  servo[0].SetTrim(0);
  servo[1].SetTrim(0);
  servo[2].SetTrim(0);
  servo[3].SetTrim(0);
  for(int i=0;i<4;i++){
    delay(100);
    servo[i].SetPosition(90);
  }
}

void dance(){
  while(true){
    servo[0].SetPosition(50);
    servo[1].SetPosition(50);
    delay(500);
    servo[0].SetPosition(100);
    servo[1].SetPosition(100);
    delay(500);
  }
}


