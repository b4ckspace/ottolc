// this file manages the sensors
#include <NewPing.h>
bool _canping = true;
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
		if((now-lastping)>100)
			sensorfoo();
	}else{
		if((now-lastping)>1000){
			lastping = millis();
			sensorfoo();
		}
	}
	//if obstacle: start walking back
	//if walking back and obstacle, do nothing
}

void echoCb(){
	lastping = millis();
	if(sonar.check_timer()){
		Serial.print(sonar.ping_result / US_ROUNDTRIP_CM);
		Serial.println("");
		_canping=true;
	}
}

void sensorfoo(){
	_canping = false;
	sonar.ping_timer(echoCb);
	lastping = millis();
}
