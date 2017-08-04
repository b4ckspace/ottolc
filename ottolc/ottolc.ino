/* This is the main project file for otto lc, the ottobot fork of the backspace ev, the laser cut version

*/

unsigned long latestAnimStep = 0;
long animation_step_ms = 30;

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  randomSeed(analogRead(7));
  initSerial();
  initMovement();
  initBuzzer();
  initAnim();
  initSensors();
  initModes();
}

void loop() {
  handleSerial();
  obstacleAvoidance();
  modecheck();
  if(!AnimEndReached()){
    if ( millis() >= latestAnimStep + animation_step_ms ){
      AnimStep(animation_step_ms);
      latestAnimStep = millis();
    }
  }
  softTone();
}
