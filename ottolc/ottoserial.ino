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
  Serial.println("setting a servo");
  int servonr;
  int degree;
  int matches = sscanf(args.c_str(), "%d %d", &servonr, &degree);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
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
      Serial.print("Unknknown servo nr ");
      Serial.println(servonr, DEC);
    break;
  }
  setServo(servo, degree);
  Serial.println("Ok");
}

void setTVar(String args){
  Serial.println("setting testvar");
  int varnr;
  int value;
  int matches = sscanf(args.c_str(), "%d %d", &varnr, &value);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  testvars[varnr] = value;
  Serial.println("Ok");
}


void replSetTrim(String args){
  Serial.println("setting servo trim");
  int servonr=-1;
  int trimvalue=-1;
  int matches = sscanf(args.c_str(), "%d %d", &servonr, &trimvalue);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  Serial.print("setting servo to: ");
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
      Serial.print("Unknknown servo nr ");
      Serial.println(servonr, DEC);
    break;
  }
  setTrimdata(servo, trimvalue);
  Serial.println("Ok");
}

void replTrimtest(){
  for(int i=0;i<4;i++){
    servo[i].SetPosition(60);
  }
  delay(500);
  resetServos();
}

void replPrintServos(){
  Serial.println("");
  Serial.print("rightFoot = ");
  Serial.print(getServo(rightFoot));
  Serial.println(";");
  Serial.print("leftFoot = ");
  Serial.print(getServo(leftFoot));
  Serial.println(";");
  Serial.print("rightLeg = ");
  Serial.print(getServo(rightLeg));
  Serial.println(";");
  Serial.print("leftLeg = ");
  Serial.print(getServo(leftLeg));
  Serial.println(";");
}

void doCommands(){
  digitalWrite(13, HIGH);
  Serial.println("Hello my name is otto, version 0.0.1");
  Serial.readStringUntil('\n'); // Clear serial buffer
  Serial.setTimeout(5l*60l*1000l);
  while(true){
    Serial.print("enter command: "); 
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
      Serial.println("servos reseted");
    }else if(command=="beep"){
      beep();
    }else if(command=="trimtest"){
      replTrimtest();
    }else if(command=="play"){
      splayAnim();
    }else if(command=="q1"){
      anim1();
    }else if(command=="q2"){
      anim2();
    }else if(command=="settrim"){
      replSetTrim(args);
      replTrimtest();
    }else if(command=="printservos"||command=="ps"){
      replPrintServos();
    }else if(command=="s"){
      sanimstep();
    }else if(command=="w"){
      sanimSetup();
    }else if(command=="tset"){
      setTVar(args);
    }else if(command=="reset"){
      void(* resetFunc) (void) = 0;
      resetFunc();
    }else if(command=="!"){
      apiCommand(args);
    }else{
      Serial.print("command not found: ");
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
    result = "empty api command";
    goto printres;
  }


  {
    auto pos = line.indexOf(' ');
    String args = "";
    if(pos<0){
      pos = line.length();
    }
    String command = line.substring(0, pos);
    //Serial.print("xxx"+command + "xxx ");
    
    if( (pos+1)<(line.length()) ){
      args = line.substring(pos+1, line.length());
    }
    if(command=="ping"){
      result="pong";
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
      int a,b,c,d;
      Serial.println(args);
      if(sscanf(args.c_str(), "%d %d %d %d", &a, &b, &c, &d)!=4){
        returncode = -1;
        result="could not parse format";
      }else{
        setServo(rightFoot, a);
        setServo(leftFoot, b);
        setServo(rightLeg, c);
        setServo(leftLeg, d);
        result="servos set";
      }
    }else{
      returncode = -1;
      result = "command not found";
    }
  }


  printres:
  memset(printbuf,0,PBUFSZ);
  snprintf(printbuf, PBUFSZ, "%d %s\n", returncode, result.c_str());
  Serial.print(printbuf);
}

void anim1(){
  Serial.println("start anim test");
  AnimKeyframe kf;

  kf.rightFoot=90;
  kf.leftFoot=90;
  kf.duration=500;
  queueFrame(kf);

  kf.duration=1000;

  kf.rightFoot = 50;
  kf.leftFoot = 60;
  kf.rightLeg = 90;
  kf.leftLeg = 90;
  // kf.duration=testvars[;
  queueFrame(kf);
  kf.leftFoot = 60;
  kf.duration = 500;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot=90;
  kf.leftFoot=90;
  kf.duration=500;
  queueFrame(kf);
}

void anim2(){
  AnimKeyframe kf;

  kf.rightFoot=90;
  kf.leftFoot=90;
  kf.duration=1;
  queueFrame(kf);

  kf.leftFoot=70;
  kf.duration=100;
  queueFrame(kf);

  kf.rightFoot=50;
  kf.duration=100;
  queueFrame(kf);

  kf.leftFoot=50;
  queueFrame(kf);

  kf.rightFoot=100;
  kf.duration=100;
  queueFrame(kf);
  kf.rightFoot=80;
  kf.duration=100;
  queueFrame(kf);

  kf.duration=200;
  queueFrame(kf);


  kf.rightFoot=100;
  kf.duration=300;
  kf.duration=300;
  queueFrame(kf);
  kf.rightFoot=70;
  queueFrame(kf);
  kf.rightFoot=110;
  queueFrame(kf);
  kf.rightFoot=70;
  queueFrame(kf);
  kf.rightFoot=110;
  queueFrame(kf);
  kf.rightFoot=70;
  queueFrame(kf);
}


void splayAnim(){
  while(!AnimEndReached()){
    AnimStep(testvars[0]);
    delay(testvars[0]);
  }

  Serial.println("end anim test");
}

void sanimSetup(){
  Serial.println("qing");
  AnimKeyframe kf;

  kf.rightFoot=90;
  kf.leftFoot=90;
  kf.duration=500;
  queueFrame(kf);

  kf.duration=500;

  kf.rightFoot = 50;
  kf.leftFoot = 70;
  kf.rightLeg = 90;
  kf.leftLeg = 90;
  // kf.duration=testvars[;
  queueFrame(kf);
  kf.leftFoot = 60;
  kf.duration = 300;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot = 130;
  queueFrame(kf);
  kf.rightFoot = 60;
  queueFrame(kf);
  kf.rightFoot=90;
  kf.leftFoot=90;
  kf.duration=500;
  queueFrame(kf);

}
void sanimstep(){
  if(!AnimEndReached()){
    AnimStep(testvars[0]);
    Serial.println("ok");
  }else{
    Serial.println("end anim test");
  }
}