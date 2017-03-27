/* This is the main project file for otto lc, the ottobot fork of the backspace ev, the laser cut version

*/

// changable by sending some data via serial during the first 2 seconds after servo reset
bool debug_mode = false;

void setup() {
  initMovement();
  initBuzzer();
  debug_mode = isDebug();
}

void loop() {
  if(debug_mode){
    doCommands();
  }else{
    dance();
  }
}
