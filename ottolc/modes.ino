// this file manages the different otto modes
#include "Bounce2.h"
#include "enums.h"

bool addAnimationCallback(AnimationCallback::AnimFun callback);

Bounce button;
int _currentmode = 3;
int _nummodes = 4;
void initModes(){
	pinMode(6,INPUT_PULLUP);
	button.attach(6);
	modeinit();
}

void modecheck(){
	if(button.update()){
		if(!button.read()){
			_currentmode = (_currentmode+1)%_nummodes;
			Serial.print("new mode: ");
			Serial.print(_currentmode);
			Serial.println();
			modeinit();
		}
	}
}
void resetServos();
void modeinit(){
	resetAnimation();
	resetServos();
	switch(_currentmode){
		case 0:
			playMelodyPart(0,8,9);
			delay(200);
			enableCollision();
			addAnimationCallback(keep_walking);
			startAnimation();
		break;
		case 1:
			playMelodyPart(0,8,11);
      delay(200);
			disableCollision();
			addAnimationCallback(keep_walking_back);
			startAnimation();
			break;
		case 2:
			playMelodyPart(0,8,13);
      delay(200);
			enableCollision();
			addAnimationCallback(randomAction);
			startAnimation();
			break;
		case 3:
			disableCollision();break;
	}
	//insert switch case for modes here
}
void mode_onCollision(){
	disableCollision();
	playMelodyPart(0,0,7);
	prependAnimationCallback(walk_back_collision);
}

void mode_onWalkbackEnd(void*){
	enableCollision();
}
