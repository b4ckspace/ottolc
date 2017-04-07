#include "enums.h"
#include <string.h>

#define ARBSIZE 20

AnimKeyframe frames[ARBSIZE];
AnimKeyframe *rhead;
AnimKeyframe *whead;
int16_t animPos;
AnimKeyframe startpos;

bool animend;

void initAnim(){
    rhead = &frames[0];
    whead = &frames[1];
    animPos=1;
    rhead->duration=1;
    rhead->rightFoot=90;
    rhead->leftFoot=90;
    rhead->rightLeg=90;
    rhead->leftLeg=90;

    startpos.duration=1;
    startpos.rightFoot=90;
    startpos.leftFoot=90;
    startpos.rightLeg=90;
    startpos.leftLeg=90;

    animend = true;
}

void queueFrame(AnimKeyframe frame);
void queueFrame(AnimKeyframe frame){
    if(whead==rhead){
        Serial.println("Animation ringbuffer full, dropping Animation");
        beep();
        return;
    }
    memcpy(whead, &frame, sizeof(AnimKeyframe));
    whead++;
    if(whead==&frames[ARBSIZE]){
        whead=&frames[0];
    }
    animend = false;
}


int getServo(EServo servon);
void setServo(EServo servon, int value);

void AnimStep(int16_t ms){
    uint16_t endtime = min(animPos+ms, rhead->duration);
    float factor = float(endtime) / float(rhead->duration);

    /*
    setServo(rightFoot, startpos.rightFoot + int16_t(round( (float(rhead->rightFoot)-float(startpos.rightFoot))*factor)) );
    Serial.print("anim step ");
    Serial.print(startpos.rightFoot);
    Serial.print(" ");
    Serial.print(rhead->rightFoot);
    //*/

    setServo(rightFoot, startpos.rightFoot + int16_t(round( (float(rhead->rightFoot)-float(startpos.rightFoot))*factor)) );
    setServo(leftFoot,  startpos.leftFoot  + int16_t(round( (float(rhead->leftFoot) -float(startpos.leftFoot)) *factor)) );
    setServo(rightLeg,  startpos.rightLeg  + int16_t(round( (float(rhead->rightLeg) -float(startpos.rightLeg)) *factor)) );
    setServo(leftLeg,   startpos.leftLeg   + int16_t(round( (float(rhead->leftLeg)  -float(startpos.leftLeg))  *factor)) );

    animPos += ms;

    if(animPos>rhead->duration){
        if( (rhead==(whead-1))||((rhead==&frames[ARBSIZE-1])&&(whead==&frames[0]) ) ){
            Serial.println("Animation ringbuffer empty, staying in last pos");
            animend = true;
            return;
        }
        memcpy(&startpos, rhead, sizeof(AnimKeyframe));
        rhead++;
        if(rhead>=&frames[ARBSIZE]){
            rhead=&frames[0];
        }
        animPos = 0;
    }
}

inline bool AnimEndReached(){
    return animend;
}