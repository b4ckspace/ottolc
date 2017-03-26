// this file manages serial communications and command dispatch for debug&adjustment purposes
#include <stdio.h>
bool is_debug(){
  Serial.begin(9600);
  while(!Serial){}
  delay(2000);
  return Serial.available();
}

void setservo(String args){
  Serial.println("setting a servo");
  int servonr;
  int degree;
  int matches = sscanf(args.c_str(), "%d %d", &servo, &degree);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  servo[servonr].SetPosition(degree);
  Serial.println("Ok");
}

void do_commands(){
  digitalWrite(13, HIGH);
  Serial.println("Hello my name is otto, version 0.0.1");
  Serial.setTimeout(5l*60l*1000l);
  while(true){
    Serial.print("enter command: "); 
    String line = Serial.readStringUntil('\n');
    if(line.length()==0){continue;}
    auto pos = line.indexOf(' ');
    if(pos<0){
      Serial.println("could not find space in command.");
      pos = line.length();
    }
    String command = line.substring(0, pos);
    Serial.print("command: ");
    Serial.println(command);
    String args = "";
    if( (pos+1)<(line.length()) ){
      args = line.substring(pos+1, line.length());
    }

    //TODO: replace with something better, hash map?
    if(command=="ping"){
      Serial.println("pong!");
    }else if(command=="setservo"){
      setservo(args);
    }else{
      Serial.print("command not found: ");
      Serial.println(command);
    }
    
  }
  //loop: read line
  //parse line(command)
  //call command with rest of parameters
  //command does the stuff
}
 
