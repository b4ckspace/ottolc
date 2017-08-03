// this file manages the different otto modes
#include "Bounce2.h"
#include "enums.h"

bool addAnimationCallback(AnimationCallback::AnimFun callback);

Bounce button;
int _currentmode = 2;
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
			// playMelodyPart(0,8,9);
			enableCollision();
			addAnimationCallback(keep_walking);
			startAnimation();
		break;
		case 1:
			// playMelodyPart(0,8,11);
			enableCollision();
			addAnimationCallback(keep_walking_back);
			startAnimation();
		case 2:
			// playMelodyPart(0,8,13);
			disableCollision();
			addAnimationCallback(randomAction);
			startAnimation();
		case 3:
			enableCollision();break;
	}
	//insert switch case for modes here
}
void mode_onCollision(){
	// Serial.println("coll");
	if(_currentmode==0){
		// setTone(262, 1000000);
		disableCollision();
		prependAnimationCallback(walk_back_collision);
	}
	//insert switch case here for collision handling
}

void mode_onWalkbackEnd(void*){
	enableCollision();
}