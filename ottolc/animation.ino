#include "enums.h"
#include <string.h>

#define ARBSIZE 50
//TODO: add special frame with function pointer to a fucntion that prepends 

AnimStep_t frames[ARBSIZE];
AnimStep_t *rhead;
AnimStep_t *whead;
int16_t animPos;
AnimKeyframe startpos;

bool animend;

void initAnim(){
    whead = &frames[1];
    resetAnimation();
}


int getServo(EServo servon);
void setServo(EServo servon, int value);
bool in_anim_callback = false;
void AnimStep(int16_t ms){
    if(!(rhead->options & (1<<AnimStep_t::IS_FRAME))){
        in_anim_callback = true;
        rhead++;
        //copy funptr and args so we dont overwrite data on front insert
        AnimationCallback cbinfo = rhead->data.cb;
        cbinfo.fun(cbinfo.args);
        in_anim_callback = false;
    }

    uint16_t endtime = min(animPos+ms, rhead->data.keyframe.duration);
    float factor = float(endtime) / float(rhead->data.keyframe.duration);

    setServo(rightFoot, startpos.rightFoot + int16_t(round( (float(rhead->data.keyframe.rightFoot)-float(startpos.rightFoot))*factor)) );
    setServo(leftFoot,  startpos.leftFoot  + int16_t(round( (float(rhead->data.keyframe.leftFoot) -float(startpos.leftFoot)) *factor)) );
    setServo(rightLeg,  startpos.rightLeg  + int16_t(round( (float(rhead->data.keyframe.rightLeg) -float(startpos.rightLeg)) *factor)) );
    setServo(leftLeg,   startpos.leftLeg   + int16_t(round( (float(rhead->data.keyframe.leftLeg)  -float(startpos.leftLeg))  *factor)) );

    animPos += ms;

    if(animPos>rhead->data.keyframe.duration){
        if( (rhead==(whead-1))||((rhead==&frames[ARBSIZE-1])&&(whead==&frames[0]) ) ){
            Serial.println(F("Animation ringbuffer empty, staying in last pos"));
            animend = true;
            return;
        }
        memcpy(&startpos, &rhead->data.keyframe, sizeof(AnimKeyframe));
        rhead++;
        if(rhead>=&frames[ARBSIZE]){
            rhead=&frames[0];
        }
        animPos = 0;
    }
}

void resetAnimation(){
    rhead = whead -1;
    if(rhead < frames){
        rhead = &frames[ARBSIZE-1];
    }
    animPos=1;
    rhead->options = 1<<AnimStep_t::IS_FRAME;
    rhead->data.keyframe.duration=500;
    rhead->data.keyframe.rightFoot=90;
    rhead->data.keyframe.leftFoot=90;
    rhead->data.keyframe.rightLeg=90;
    rhead->data.keyframe.leftLeg=90;

    startpos.duration=1;
    startpos.rightFoot=90;
    startpos.leftFoot=90;
    startpos.rightLeg=90;
    startpos.leftLeg=90;

    animend = true;
}
bool addAnimationFrame(uint8_t leftFoot, uint8_t rightFoot, uint8_t leftLeg, uint8_t rightLeg, uint16_t time){
    if(whead==rhead){
        Serial.println(F("Animation ringbuffer full, dropping Animation"));
        beep();
        return 1;
    }
    whead->options = 1<<AnimStep_t::IS_FRAME;
    whead->data.keyframe.duration  = time;
    whead->data.keyframe.rightFoot = rightFoot;
    whead->data.keyframe.leftFoot  = leftFoot;
    whead->data.keyframe.rightLeg  = rightLeg;
    whead->data.keyframe.leftLeg   = leftLeg;
    whead++;
    if(whead==&frames[ARBSIZE]){
        whead=&frames[0];
    }
    return 0;
}

bool prependAnimationFrame(uint8_t leftFoot, uint8_t rightFoot, uint8_t leftLeg, uint8_t rightLeg, uint16_t time){
    if(!in_anim_callback){
        Serial.println(F("prepend called but not in animation callback"));
        beep();
        return 1;
    }

    if(whead==rhead){
        Serial.println(F("Animation ringbuffer full, dropping Animation"));
        beep();
        return 1;
    }

    rhead--;

    if(rhead<&frames[0]){
        rhead=&frames[ARBSIZE-1];
    }
    if(whead==rhead){
        Serial.println(F("Animation ringbuffer full, dropping Animation"));
        beep();
        return 1;
    }

    rhead->options = 1<<AnimStep_t::IS_FRAME;
    rhead->data.keyframe.duration  = time;
    rhead->data.keyframe.rightFoot = rightFoot;
    rhead->data.keyframe.leftFoot  = leftFoot;
    rhead->data.keyframe.rightLeg  = rightLeg;
    rhead->data.keyframe.leftLeg   = leftLeg;
    return 0;
}

void startAnimation(){
    animend = false;
}


inline bool AnimEndReached(){
    return animend;
}