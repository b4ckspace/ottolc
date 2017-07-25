// this file manages general movement and servo comms
//#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include "enums.h"

#define N_SERVOS 4


#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2

Oscillator servo[N_SERVOS];
int servopos[N_SERVOS];

void resetServos();
void verifyTrimdata();
char readSetting(Setting key);
void writeSetting(Setting key, char value);

// Caclulate absolute servo position
int absPos(int pos = 0){
  pos += 90;
  return pos;
}
// Caclulate relative servo position
int relPos(int pos = 0){
  pos -= 90;
  return pos;
}

void initMovement(){
  for(int i=0;i<N_SERVOS;i++){
    servopos[i]=absPos();
  }
  enableServos();
  resetServos();
}
void enableServos(){
  servo[0].attach(PIN_RR);
  servo[0].SetPosition(servopos[0]);
  servo[1].attach(PIN_RL);
  servo[1].SetPosition(servopos[1]);
  servo[2].attach(PIN_YR);
  servo[2].SetPosition(servopos[2]);
  servo[3].attach(PIN_YL);
  servo[3].SetPosition(servopos[3]);
}
void disableServos(){
  servo[0].detach();
  servo[1].detach();
  servo[2].detach();
  servo[3].detach();
}

void resetServos();
void resetServos(){
  verifyTrimdata();
  servo[0].SetTrim(readSetting(trimRightFoot));
  servo[1].SetTrim(readSetting(trimLeftFoot));
  servo[2].SetTrim(readSetting(trimRightLeg));
  servo[3].SetTrim(readSetting(trimLeftLeg));
  for(int i=0;i<N_SERVOS;i++){
    servo[i].SetPosition(90);
    servopos[i]=90;
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

void setServo(EServo servon, int value);
void setServo(EServo servon, int value){
  int idx=-1;
  switch(servon){
    case rightFoot: idx=0;break;
    case leftFoot:  idx=1;break;
    case rightLeg:  idx=2;break;
    case leftLeg:   idx=3;break;
    default:Serial.println(F("could not match servo in setservo oO"));break;
  }
  if(value<0 || value > 180 ){
    Serial.print(F("preventing bad servo value "));
    Serial.print(idx);
    Serial.print(F(" "));
    Serial.println(value);
    return;
  }
  
  servo[idx].SetPosition(value);
  servopos[idx] = value;
}

int getServo(EServo servon);
int getServo(EServo servon){
  int idx=-1;
  switch(servon){
    case rightFoot: idx=0;break;
    case leftFoot:  idx=1;break;
    case rightLeg:  idx=2;break;
    case leftLeg:   idx=3;break;
  }
  return servopos[idx];
}

void verifyTrimdata(){
  char a = readSetting(trimLeftFoot);
  char b = readSetting(trimRightFoot);
  char c = readSetting(trimLeftLeg);
  char d = readSetting(trimRightLeg);
  char chk = readSetting(trimChecksum);
  char data[4] = {a,b,c,d};
  char chk2 = badChecksum(data, 4);
  if(chk!=chk2){
    Serial.println(F("Trimdata checksum verification failed."));
    char zdata[] = {0,0,0,0};
    char zsum = badChecksum(zdata, 4);
    writeSetting(trimLeftFoot, 0);
    writeSetting(trimRightFoot, 0);
    writeSetting(trimLeftLeg, 0);
    writeSetting(trimRightLeg, 0);
    writeSetting(trimChecksum, zsum);
  }else{
    Serial.println(F("Trim data checksum ok"));
  }
}

Setting servoToTrimSetting(EServo servo);
Setting servoToTrimSetting(EServo servo){
  switch(servo){
    case rightFoot: return trimRightFoot;
    case leftFoot: return trimLeftFoot;
    case rightLeg: return trimRightLeg;
    case leftLeg: return trimLeftLeg;
  }
}

void setTrimdata(EServo servo, char trimval);
void setTrimdata(EServo servo, char trimval){
  verifyTrimdata();
  auto setting = servoToTrimSetting(servo);
  writeSetting(setting, trimval);
  char a = readSetting(trimLeftFoot);
  char b = readSetting(trimRightFoot);
  char c = readSetting(trimLeftLeg);
  char d = readSetting(trimRightLeg);
  char data[4] = {a,b,c,d};
  char chk = badChecksum(data, 4);
  writeSetting(trimChecksum, chk);
}

char getTrimdata(EServo servo);
char getTrimdata(EServo servo){
  auto setting = servoToTrimSetting(servo);
  return readSetting(setting);
}

// TODO: replace with something good
char badChecksum(char* data, size_t len){
  char ret = 2;
  for(size_t i=0;i<len;i++){
    char b = data[i];
    if(i%2){
      b = b ^ char(i);
    }else{
      b = b + char(i);
    }
    ret = ret ^ b;
  }
  return ret;
}
