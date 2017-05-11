/* This is the main project file for otto lc, the ottobot fork of the backspace ev, the laser cut version

*/
#include <Ultrasonic.h>

bool debug_mode = false; /* changable by sending some data via serial during the first 2 seconds after servo reset */
unsigned int distance = 0;
Ultrasonic ultrasonic(8, 9); /* create ultrasonic object with trigger and echo pin definition */

#define DANCE_STOP_DISTANCE_LIMIT 15 /* obstacle nearer than this (cm) lets otto stop dancing */

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  initMovement();
  initBuzzer();
  debug_mode = isDebug();
}

void loop() {
  if(debug_mode){
    doCommands();
  }else{
    distance = ultrasonic.distanceRead();
    if (distance >= DANCE_STOP_DISTANCE_LIMIT) /* no obstacle near */
    {
    	dance();
    }
    else /* there is an obstacle */
    {
    	Serial.print("Distance < ");
    	Serial.print(DANCE_STOP_DISTANCE_LIMIT);
    	Serial.println(" cm! --> Reset.");
    	delay(100);
    	setup(); /* initialize again to be able to enter debug mode again */
    }
  }
}
