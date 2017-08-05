// this file manages serial communications and command dispatch for debug&adjustment purposes
#include <stdio.h>
bool initSerial(){
  Serial.setTimeout(20);
  Serial.println(F("I am otto, version 0.0.1"));
  Serial.readStringUntil('\n'); // Clear serial buffer
  Serial.println(F(".0 boot_ok"));
}

int apiVersion = 3;
int fwVersion = 2;

void playTone(String args){
  int myTone;
  int duration;
  int matches = sscanf(args.c_str(), "%d %d", &myTone, &duration);
  setTone(myTone, duration * (unsigned long)100000);
}

void setServo(EServo servo, int value);
void replSetServo(String args){
  Serial.println(F("setting a servo"));
  int servonr;
  int degree;
  int matches = sscanf(args.c_str(), "%d %d", &servonr, &degree);
  if(matches!=2){
    Serial.print(matches);
    Serial.print(F("could not parse arguments: '"));
    Serial.print(args);
    Serial.println(F("'"));
    return;
  }
  EServo servo;
  switch(servonr){
    case 0:
      servo = rightFoot; break;
    case 1:
      servo = leftFoot; break;
    case 2:
      servo = rightLeg; break;
    case 3:
      servo = leftLeg; break;
    default:
      Serial.print(F("Unknknown servo nr "));
      Serial.println(servonr, DEC);
    break;
  }
  setServo(servo, degree);
  Serial.println(F("Ok"));
}


void replSetTrim(String args){
  Serial.println(F("setting servo trim"));
  int servonr=-1;
  int trimvalue=-1;
  int matches = sscanf(args.c_str(), "%d %d", &servonr, &trimvalue);
  if(matches!=2){
    Serial.print(matches);
    Serial.print(F("could not parse arguments: '"));
    Serial.print(args);
    Serial.println(F("'"));
    return;
  }
  Serial.print(F("setting servo to: "));
  Serial.print(servonr);
  Serial.println(trimvalue);
  EServo servo;
  switch(servonr){
    case 0:
      servo = rightFoot; break;
    case 1:
      servo = leftFoot; break;
    case 2:
      servo = rightLeg; break;
    case 3:
      servo = leftLeg; break;
    default:
      Serial.print(F("Unknknown servo nr "));
      Serial.println(servonr, DEC);
    break;
  }
  setTrimdata(servo, trimvalue);
  Serial.println(F("Ok"));
}

void replTrimtest(){
  for(int i=0;i<4;i++){
    servo[i].SetPosition(65);
  }
  delay(500);
  resetServos();
}

void replPrintServos(){
  Serial.println(F(""));
  Serial.print(F("rightFoot = "));
  Serial.print(getServo(rightFoot));
  Serial.println(F(";"));
  Serial.print(F("leftFoot = "));
  Serial.print(getServo(leftFoot));
  Serial.println(F(";"));
  Serial.print(F("rightLeg = "));
  Serial.print(getServo(rightLeg));
  Serial.println(F(";"));
  Serial.print(F("leftLeg = "));
  Serial.print(getServo(leftLeg));
  Serial.println(F(";"));
}

//we do our own input handling so we do not block if there is an incomplete
//line in the buffer
String linebuf;
void handleSerial(){
  while(Serial.available()>0){
    char chr = Serial.read();
    switch(chr){
      case '\r':break;
      case '\n':
        doCommand(linebuf);
        linebuf="";
      break;
      default:
        linebuf+=chr;
    }
  }
}

void doCommand(String line){
  if(line.length()==0){
    return;
  }
  auto pos = line.indexOf(' ');
  if(pos<0){
    pos = line.length();
  }
  String command = line.substring(0, pos);
  Serial.print(command + " ");
  String args = "";
  if( (pos+1)<(line.length()) ){
    args = line.substring(pos+1, line.length());
  }

  //TODO: replace with something better, hash map?
  if(command==F("ping")){
    Serial.println("pong!");
  }else if(command==F("setservo")){
    replSetServo(args);
  }else if(command==F("resetservos")){
    resetServos();
    //Serial.println(F("servos reseted"));
  }else if(command==F("mel0")){
    playMelody(0);
  }else if(command==F("mel1")){
    playMelody(1);
  }else if(command==F("mel2")){
    playMelodyPart(1,0,3);
  }else if(command==F("beep1")){
    playMelodyPart(0,0,7);
  }else if(command==F("beep2")){
    playMelodyPart(0,8,13);
  }else if(command==F("beep3")){
    setTone(250, 1000000);
  }else if(command==F("play")){
    startAnimation();
  }else if(command==F("st")){  // settrim
    replSetTrim(args);
    replTrimtest();
  }else if(command==F("ps")){  // settrim
    replPrintServos();
  }else if(command=="e"){
    Serial.println(F("echo? "));
    sensorfoo();
  }else if(command==F("reset")){
    void(* resetFunc) (void) = 0;
    resetFunc();
  }else if(command=="!"){
    apiCommand(args);
  }else{
    Serial.print(F("command not found: "));
    Serial.println(command);
    // beep();
  }
  Serial.print(F("enter command: ")); 
}

// this seperate function should be used for commands that are used by tools and should not change their interface
// commands sould return a non 0 value on error and a message 
#define PBUFSZ 200
char printbuf[PBUFSZ];

void apiCommand(String line){
  // API DESIGN:
  // to distinguish between debug output and api output on the serial connection
  // api call results are prefixed by a dot .
  // the . is then followed by the return code integer, 0 representing ok and all other
  // values representing an error
  // this is then followed by either the function call result or
  // a human readable error string.

  Serial.println();
  int returncode=0;
  String result="";
  
  if(line.length()==0){
    returncode = -1;
    result = F("empty api command");
    goto printres;
  }


  {
    auto pos = line.indexOf(' ');
    String args = "";
    if(pos<0){
      pos = line.length();
    }
    String command = line.substring(0, pos);
    
    if( (pos+1)<(line.length()) ){
      args = line.substring(pos+1, line.length());
    }
    if(command==F("ping")){
      result="pong";
    }else if(command==F("maxframes")){
      result=ARBSIZE;
    }else if(command==F("apiversion")){
      result = apiVersion;
    }else if(command==F("fwversion")){
      result = fwVersion;
    }else if(command==F("getinfo")){
      memset(printbuf,0,PBUFSZ);
      snprintf(printbuf, PBUFSZ, "%d %d %s", fwVersion, apiVersion, __DATE__);
      //snprintf(printbuf, PBUFSZ, "Versions 1 2");
      result=printbuf;
    }else if(command==F("gettrims")){
      char getTrimdata(EServo servo);
      memset(printbuf,0,PBUFSZ);
      snprintf(printbuf, PBUFSZ, "%d %d %d %d", getTrimdata(rightFoot), getTrimdata(leftFoot), getTrimdata(rightLeg), getTrimdata(leftLeg));
      result=printbuf;
    }else if(command==F("settrims")){
      int a,b,c,d;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d", &a, &b, &c, &d)!=4){
        returncode = -1;
        result="could not parse format";
      }else{
        setTrimdata(rightFoot, a);
        setTrimdata(leftFoot, b);
        setTrimdata(rightLeg, c);
        setTrimdata(leftLeg, d);
        result="trim data set";
      }
    }else if(command==F("trimtest")){
      replTrimtest();
    }else if(command=="getservos"){
      int getServo(EServo servo);
      memset(printbuf,0,PBUFSZ);
      snprintf(printbuf, PBUFSZ, "%d %d %d %d", relPos(getServo(rightFoot)), relPos(getServo(leftFoot)), relPos(getServo(rightLeg)), relPos(getServo(leftLeg)));
      result=printbuf;
    }else if(command==F("setservos")){
      int leftFootv,rightFootv,leftLegv,rightLegv;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d", &rightFootv, &leftFootv, &rightLegv, &leftLegv)!=4){
        returncode = -1;
        result="could not parse format";
      }else{
        setServo(rightFoot, absPos(rightFootv));
        setServo(leftFoot, absPos(leftFootv));
        setServo(rightLeg, absPos(rightLegv));
        setServo(leftLeg, absPos(leftLegv));
        result="Servos set";
      }
    }else if(command==F("pushframe")){
      int leftFootv,rightFootv,leftLegv,rightLegv,duration;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d %d", &rightFootv, &leftFootv, &rightLegv, &leftLegv, &duration)!=5){
        returncode = -1;
        result="could not parse format";
      }else{
        addAnimationFrame(absPos(leftFootv),absPos(rightFootv),absPos(leftLegv),absPos(rightLegv),duration);
        result="frame added";
      }
    }else if(command==F("resetanim")){
      result = "animation cleared";
      resetAnimation();
    }else if(command==F("playanim")){
      startAnimation();
    // the gui can use this to query the build in functions that can be used as animation callback
    }else if(command==F("supportedanims")){
      result = String(F("basic_step basic_back kneel_down moonwalk_r huffing turnleft turnright"));

    // push a callback instead of a keyframe
    }else if(command==F("pushcallback")){
      bool addAnimationCallback(AnimationCallback::AnimFun callback);

      bool found = false;
      if(args==F("basic_step")){
        found=true;
        addAnimationCallback(basic_step);
      }else if(args==F("basic_back")){
        found=true;
        addAnimationCallback(basic_back);
      }else if(args==F("kneel_down")){
        found=true;
        addAnimationCallback(kneel_down);
      }else if(args==F("moonwalk_r")){
        found=true;
        addAnimationCallback(moonwalk_r);
      }else if(args==F("huffing")){
        found=true;
        addAnimationCallback(huffing);
      }else if(args==F("turnleft")){
        found=true;
        addAnimationCallback(turnleft);
      }else if(args==F("turnright")){
        found=true;
        addAnimationCallback(turnright);
      }
      if(!found){
        result = "no callback with that name found";
        returncode = 23;
      }

    }else if(command==F("servosoff")){
      disableServos();
      result = "servos switched off";
    }else if(command==F("servoson")){
      enableServos();
      result = "servos switched on";
    }else if(command==F("beep")){
      setTone(262, 1000000); //C4
    }else if(command==F("tone")){
      playTone(args);
    }else if(command==F("beep_m1")){
      playMelodyPart(0,8,9);
    }else if(command==F("beep_m2")){
      playMelodyPart(0,8,11);
    }else if(command==F("beep_m3")){
      playMelodyPart(0,8,13);
    }else{
      returncode = -1;
      result = "command not found";
    }
  }


  printres:
  memset(printbuf,0,PBUFSZ);
  snprintf(printbuf, PBUFSZ, ".%d %s\r\n", returncode, result.c_str());
  Serial.print(printbuf);
}
