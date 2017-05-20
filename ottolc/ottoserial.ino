// this file manages serial communications and command dispatch for debug&adjustment purposes
#include <stdio.h>
bool isDebug(){
  //beep();
  return true;
  delay(2000);
  return Serial.available();
}

int testvars[10] = {10,500,500,0,0,0,0,0,0,0};

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

void setTVar(String args){
  Serial.println(F("setting testvar"));
  int varnr;
  int value;
  int matches = sscanf(args.c_str(), "%d %d", &varnr, &value);
  if(matches!=2){
    Serial.print(matches);
    Serial.print(F("could not parse arguments: '"));
    Serial.print(args);
    Serial.println("'");
    return;
  }
  testvars[varnr] = value;
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
    servo[i].SetPosition(60);
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

void doCommands(){
  digitalWrite(13, HIGH);
  Serial.println(F("Hello my name is otto, version 0.0.1"));
  Serial.readStringUntil('\n'); // Clear serial buffer
  Serial.setTimeout(5l*60l*1000l);
  while(true){
    Serial.print(F("enter command: ")); 
    String line = Serial.readStringUntil('\n');
    if(line.length()==0){
      Serial.println();
      continue;
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
    if(command=="ping"){
      Serial.println("pong!");
    }else if(command=="setservo"){
      replSetServo(args);
    }else if(command=="resetservos"){
      resetServos();
      Serial.println(F("servos reseted"));
    }else if(command=="beep"){
      beep();
    }else if(command=="trimtest"){
      replTrimtest();
    }else if(command=="play"){
      splayAnim();
    }else if(command=="settrim"){
      replSetTrim(args);
      replTrimtest();
    }else if(command=="printservos"||command=="ps"){
      replPrintServos();
    }else if(command=="s"){
      sanimstep();
    }else if(command=="tset"){
      setTVar(args);
    }else if(command=="reset"){
      void(* resetFunc) (void) = 0;
      resetFunc();
    }else if(command=="!"){
      apiCommand(args);
    }else{
      Serial.print(F("command not found: "));
      Serial.println(command);
      beep();
    }
    
  }
}

// this seperate function should be used for commands that are used by tools and should not change their interface
// commands sould return a non 0 value on error and a message 
#define PBUFSZ 200
char printbuf[PBUFSZ];

void apiCommand(String line){
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
    if(command=="ping"){
      result="pong";
    }else if(command=="maxframes"){
      result=ARBSIZE;
    }else if(command=="apiversion"){
      result="0001";
    }else if(command=="gettrims"){
      char getTrimdata(EServo servo);
      memset(printbuf,0,PBUFSZ);
      snprintf(printbuf, PBUFSZ, "+ %d %d %d %d", getTrimdata(rightFoot), getTrimdata(leftFoot), getTrimdata(rightLeg), getTrimdata(leftLeg));
      result=printbuf;
    }else if(command=="settrims"){
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
    }else if(command=="getservos"){
      int getServo(EServo servo);
      memset(printbuf,0,PBUFSZ);
      snprintf(printbuf, PBUFSZ, "+ %d %d %d %d", getServo(rightFoot), getServo(leftFoot), getServo(rightLeg), getServo(leftLeg));
      result=printbuf;
    }else if(command=="setservos"){
      int leftFootv,rightFootv,leftLegv,rightLegv;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d", &rightFootv, &leftFootv, &rightLegv, &leftLegv)!=4){
        returncode = -1;
        result="could not parse format";
      }else{
        setServo(rightFoot, rightFootv);
        setServo(leftFoot, leftFootv);
        setServo(rightLeg, rightLegv);
        setServo(leftLeg, leftLegv);
        result="servos set";
      }
    }else if(command=="pushframe"){
      int leftFootv,rightFootv,leftLegv,rightLegv,duration;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d %d", &rightFootv, &leftFootv, &rightLegv, &leftLegv, &duration)!=5){
        returncode = -1;
        result="could not parse format";
      }else{
        addAnimationFrame(leftFootv,rightFootv,leftLegv,rightLegv,duration);
        result="frame added";
      }
    }else if(command=="resetanim"){
      result = "animation cleared";
      resetAnimation();
    }else if(command=="playanim"){
      while(!AnimEndReached()){
        AnimStep(testvars[0]);
        delay(testvars[0]);
      }
    }else if(command=="servosoff"){
      disableServos();
      result = "servos switched off";
    }else if(command=="servoson"){
      enableServos();
      result = "servos switched on";
    }else{
      returncode = -1;
      result = "command not found";
    }
  }


  printres:
  memset(printbuf,0,PBUFSZ);
  snprintf(printbuf, PBUFSZ, ".%d %s\n", returncode, result.c_str());
  Serial.print(printbuf);
}



void splayAnim(){
  while(!AnimEndReached()){
    AnimStep(testvars[0]);
    delay(testvars[0]);
  }

  Serial.println(F("end anim test"));
}

void sanimstep(){
  if(!AnimEndReached()){
    AnimStep(testvars[0]);
    Serial.println(F("ok"));
  }else{
    Serial.println(F("end anim test"));
  }
}