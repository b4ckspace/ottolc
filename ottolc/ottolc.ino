/* This is the main project file for otto lc, the ottobot fork of the backspace ev, the laser cut version

*/

void setup() {
  init_movement();
  // put your setup code here, to run once:
  if(is_debug()){
    do_commands();
  }else{
    dance();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
