#include <EEPROM.h>

#include "enums.h"

// this is required as it will not build in more recent arduino versions
// as they add all function prototypes at the beginning and the 
// enum will be unknown at that point. defining the prototype here
// fixes this and the build will run
char readSetting(Setting key);

char readSetting(Setting key){
  return EEPROM.read(int(key));
}
void writeSetting(Setting key, char value);
void writeSetting(Setting key, char value){
  Serial.print("writing to settings ");
  Serial.print(int(key));
  Serial.print(" ");
  Serial.println(value, DEC);
  EEPROM.write(int(key), value);
}
