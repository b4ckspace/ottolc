#include "enums.h"
#include <string.h>

#define ARBSIZE 50
//TODO: add special frame with function pointer to a fucntion that prepends 

AnimStep_t frames[ARBSIZE];
AnimStep_t *rhead;
AnimStep_t *whead;
int16_t animPos;
AnimKeyframe startpos;

inline bool can_append(){
    if(is_empty)
        return true;
    if(whead==(rhead-1))
        return false;
    if( (rhead==&frames[ARBSIZE-1]) && (whead==&frames[0]) )
        return false;
    return true;
}
inline bool can_prepend(){
    if(is_empty)
        return true;
    if(rhead==(whead-1))
        return false;
    if( (whead==&frames[ARBSIZE-1]) && (rhead==&frames[0]) )
        return false;
    return true;
}
inline bool is_empty(){
    return rhead==whead;
}
inline  bool has_next(){
    if(is_empty())
        return false;
    if(rhead==(whead-1))
        return false;
    if( (whead==&frames[0]) && (rhead==&frames[ARBSIZE-1]) )
        return false;
    return true;
}

bool animend;

void initAnim(){
    resetAnimation();
}


int getServo(EServo servon);
void setServo(EServo servon, int value);
bool in_anim_callback = false;



void AnimStep(int16_t ms){
    if(animend)
        return;
    if(!(rhead->options & (1<<AnimStep_t::IS_FRAME))){
        in_anim_callback = true;
        //copy funptr and args so we dont overwrite data on front insert
        AnimationCallback cbinfo = rhead->data.cb;
        rhead++;
        if(rhead>=&frames[ARBSIZE]){
            rhead=&frames[0];
        }
        cbinfo.fun(cbinfo.args);
        in_anim_callback = false;
        AnimStep(ms);
        return;
    }

    uint16_t endtime = min(animPos+ms, rhead->data.keyframe.duration);
    float factor = float(endtime) / float(rhead->data.keyframe.duration);

    setServo(rightFoot, startpos.rightFoot + int16_t(round( (float(rhead->data.keyframe.rightFoot)-float(startpos.rightFoot))*factor)) );
    setServo(leftFoot,  startpos.leftFoot  + int16_t(round( (float(rhead->data.keyframe.leftFoot) -float(startpos.leftFoot)) *factor)) );
    setServo(rightLeg,  startpos.rightLeg  + int16_t(round( (float(rhead->data.keyframe.rightLeg) -float(startpos.rightLeg)) *factor)) );
    setServo(leftLeg,   startpos.leftLeg   + int16_t(round( (float(rhead->data.keyframe.leftLeg)  -float(startpos.leftLeg))  *factor)) );

    animPos += ms;

    if(animPos > rhead->data.keyframe.duration){
        if(!has_next()){
            Serial.println(F("Animation ringbuffer empty, staying in last pos"));
            rhead=whead;
            animend=true;
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
    rhead = whead = &frames[0];
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

//TODO: add params
bool addAnimationCallback(AnimationCallback::AnimFun callback);
bool addAnimationCallback(AnimationCallback::AnimFun callback){
    if(!can_append()){
        Serial.println(F("Animation ringbuffer full, dropping addAnimationCallback"));
        beep();
        return 1;
    }
    whead->data.cb.fun = callback;
    whead->options = 0;
    whead++;
    if(whead==&frames[ARBSIZE]){
        whead=&frames[0];
    }
    return 0;
}

//TODO: add params
bool prependAnimationCallback(AnimationCallback::AnimFun callback);
bool prependAnimationCallback(AnimationCallback::AnimFun callback){
    if(!in_anim_callback){
        //ensure we have a new startpos which was the last set position so transition is smooth
        startpos.rightFoot = getServo(rightFoot);
        startpos.leftFoot = getServo(leftFoot);
        startpos.rightLeg = getServo(rightLeg);
        startpos.leftLeg = getServo(leftLeg);
    }
    if(!can_prepend()){
        Serial.println(F("Animation ringbuffer full, dropping prependAnimationCallback"));
        beep();
        return 1;
    }

    rhead--;

    if(rhead<&frames[0]){
        rhead=&frames[ARBSIZE-1];
    }
    rhead->data.cb.fun = callback;
    rhead->options = 0;
    return 0;
}

bool addAnimationFrame(uint8_t leftFoot, uint8_t rightFoot, uint8_t leftLeg, uint8_t rightLeg, uint16_t time){
    if(!can_append()){
        Serial.println(F("Animation ringbuffer full, dropping addAnimationFrame"));
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

    if(!can_prepend()){
        Serial.println(F("Animation ringbuffer full, dropping prependAnimationFrame"));
        beep();
        return 1;
    }

    rhead--;

    if(rhead<&frames[0]){
        rhead=&frames[ARBSIZE-1];
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
