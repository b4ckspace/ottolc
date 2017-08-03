// This file controls the buzzer for sound output

#define PIN_BUZZER 10

// Notes
// Start by defining the relationship between
//       note, period, &  frequency.
#define t_c     3830    // 261 Hz
#define t_d     3400    // 294 Hz
#define t_e     3038    // 329 Hz
#define t_f     2864    // 349 Hz
#define t_g     2550    // 392 Hz
#define t_a     2272    // 440 Hz
#define t_b     2028    // 493 Hz
#define t_C     1912    // 523 Hz

// Define a special note, 'R', to represent a rest
#define t_R     0
#define NOTE_R     0

// Some more notes
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978



#define MAX_MELODY_LENGTH 16


void initBuzzer(){
  pinMode(PIN_BUZZER,OUTPUT);
}

// Some Variables
int melody = -1;
int melBoundary[] = {-1,-1};
int note = 0;  //number of the note in melody
int tone_ = -1;
int melodyEnd = MAX_MELODY_LENGTH;
unsigned long toneChange = 0;
unsigned long toneBegin = 0;
unsigned long toneDuration = 0;
bool toneState = false;


void softTone(){
int melodies[2][MAX_MELODY_LENGTH] = {
  {NOTE_C4,  NOTE_R,  NOTE_C4,  NOTE_R,  NOTE_C4,  NOTE_R,  NOTE_C8,  NOTE_R,  NOTE_C5,  NOTE_R,  NOTE_C5,  NOTE_R,  NOTE_C5,  NOTE_R,  NOTE_R,  NOTE_R },
  {NOTE_A7,  NOTE_G7,  NOTE_E7,  NOTE_C7,  NOTE_D7,   NOTE_B7,  NOTE_F7,  NOTE_C8,  NOTE_A7,  NOTE_G7, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_B7, NOTE_F7, NOTE_C8 }
};
int beats[2][MAX_MELODY_LENGTH]  = {
  {20, 10, 20,  10,  20,  10, 80, 10, 20, 10, 20, 10, 20, 0, 0, 0 },
  {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }
};

/*
  int melodies[2][MAX_MELODY_LENGTH] = {
    {t_C,  t_b,  t_g,  t_C,  t_b,  t_e,  t_R,  t_C,  t_c,  t_g,  t_a,  t_C,  t_R,  t_R,  t_R,  t_R },
    {NOTE_A7,  NOTE_G7,  NOTE_E7,  NOTE_C7,  NOTE_D7,   NOTE_B7,  NOTE_F7,  NOTE_C8,  NOTE_A7,  NOTE_G7, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_B7, NOTE_F7, NOTE_C8 }
  };
  int beats[2][MAX_MELODY_LENGTH]  = {
    {16, 16, 16,  8,  8,  16, 32, 16, 16, 16, 8, 8, 0, 0, 0, 0 },
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }
  };
*/
  

  
  unsigned long now = micros();

  // play melody
  //Serial.print("tone> ");
  //Serial.println(tone_);
  if ( melody >=0 && tone_ == -1 ){
    // check which part of the melody should be played
    //melodyEnd = MAX_MELODY_LENGTH;
    if ( note == 0 && melBoundary[0] >= 0){
      note = melBoundary[0];
      melodyEnd = melBoundary[1];
    }
    // play it loud! ;)
    setTone(melodies[melody][note], beats[melody][note] * (unsigned long)10000);
    note++;
    if(note >= melodyEnd){
      note = 0;
      melody = -1;
      melBoundary[0] = -1;
      melBoundary[1] = -1;
      melodyEnd = MAX_MELODY_LENGTH;
    }
  }

  // play tones
  if ( now < toneBegin + toneDuration && tone_ >= 0){
    if ( now - toneChange >= tone_ / 2 ){
      if ( toneState == false && tone_ > 0){
        digitalWrite(PIN_BUZZER,HIGH);
        toneState = true;
      }else{
        digitalWrite(PIN_BUZZER,LOW);
        toneState = false;
      }
      toneChange = now;
    }
    
  }else{
    tone_ = -1;
    digitalWrite(PIN_BUZZER,LOW);
    toneState = false;
  }
}

void setTone(int myTone, unsigned long duration){
  tone_ = myTone;
  toneDuration = duration;
  toneBegin = micros();
  Serial.print("tone ");
  Serial.println(myTone);
}


void playMelody(int m){
  melody = m;
}

void playMelodyPart(int m, int s, int e){
  melody = m;
  melBoundary[0] = s;
  melBoundary[1] = e;
}



void beep(){
  setTone(500, 800000);
}


/*
void r2D2(){
beep(speakerPin, note_A7,100); //A
beep(speakerPin, note_G7,100); //G
beep(speakerPin, note_E7,100); //E
beep(speakerPin, note_C7,100); //C
beep(speakerPin, note_D7,100); //D
beep(speakerPin, note_B7,100); //B
beep(speakerPin, note_F7,100); //F
beep(speakerPin, note_C8,100); //C
beep(speakerPin, note_A7,100); //A
beep(speakerPin, note_G7,100); //G
beep(speakerPin, note_E7,100); //E
beep(speakerPin, note_C7,100); //C
beep(speakerPin, note_D7,100); //D
beep(speakerPin, note_B7,100); //B
beep(speakerPin, note_F7,100); //F
beep(speakerPin, note_C8,100); //C
}
*/











