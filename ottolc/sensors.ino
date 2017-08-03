// this file manages the sensors
#include <NewPing.h>
bool _canping = true;
const int _collisiondist = 20;
const int _safetylimit = 3;
int _safetycounter = 0;
bool _collison_enabled;

signed long lastping;
NewPing sonar(8, 9 , 50);
//uses https://bitbucket.org/teckel12/arduino-new-ping

void initSensors(){
	_canping = false;
	sonar.ping_timer(echoCb);
	lastping = millis();
}

//check_timer will return false if there was no ping reply yet
//depending weather more time than the maximum expected distance 
//has passed, it will stop the timer.
//thats why we start a new ping only if we did not receive a reply
//after a much longer time in obstacleAvoidance
//also to avoid too frequent pings if an obstacle is right in front we
//wait some time after a reply

void obstacleAvoidance(){
	signed long now = millis();
	if(_canping){
		if((now-lastping)>50)
			sensorfoo();
	}else{
		if((now-lastping)>200){
			lastping = millis();
			sensorfoo();
		}
	}
}

void echoCb(){
	lastping = millis();
	if(sonar.check_timer()){
		auto dist = sonar.ping_result / US_ROUNDTRIP_CM;
		Serial.print(dist);
		Serial.println("");
		if(_collison_enabled){
			if(dist <= _collisiondist){
				_safetycounter++;
				if(_safetycounter>=_safetylimit){
					mode_onCollision();
					_safetycounter=0;
				}
			}else{
				_safetycounter=0;
			}
		}
		_canping=true;
	}
}

void sensorfoo(){
	_canping = false;
	sonar.ping_timer(echoCb);
	lastping = millis();
}

void enableCollision(){
	_safetycounter = 0;
	_collison_enabled = true;
}

void disableCollision(){
	_collison_enabled = false;
}