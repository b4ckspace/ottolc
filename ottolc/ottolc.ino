/* This is the main project file for otto lc, the ottobot fork of the backspace ev, the laser cut version

*/


void setup() {
  Serial.begin(9600);
  while(!Serial){}
  initMovement();
  initBuzzer();
  initAnim();
  // initSensors();
}

void loop() {
  handleSerial();
  obstacleAvoidance();
  if(!AnimEndReached()){
    long animation_step_ms = 30;
    AnimStep(animation_step_ms);
    delay(animation_step_ms);
  }
}
