// This file controls the buzzer for sound output

#define PIN_BUZZER 10

// Notes
// Start by defining the relationship between
//       note, period, &  frequency.
#define  t_c     3830    // 261 Hz
#define  t_d     3400    // 294 Hz
#define  t_e     3038    // 329 Hz
#define  t_f     2864    // 349 Hz
#define  t_g     2550    // 392 Hz
#define  t_a     2272    // 440 Hz
#define  t_b     2028    // 493 Hz
#define  t_C     1912    // 523 Hz

// Define a special note, 'R', to represent a rest
#define  t_R     0

// Some more notes
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_A6  1760
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_D8  4699






void initBuzzer(){
  pinMode(PIN_BUZZER,OUTPUT);
}

// Some Variables
int melody = -1;
int note = 0;  //number of the note in melody
int tone_ = 0;
unsigned long toneChange = 0;
unsigned long toneBegin = 0;
unsigned long toneDuration = 1000 * 0;
bool toneState = false;


void softTone(){
/*
  int melodies[2][16] = {
    {t_C,  t_b,  t_g,  t_C,  t_b,  t_e,  t_R,  t_C,  t_c,  t_g,  t_a,  t_C,  t_R,  t_R,  t_R,  t_R },
    {NOTE_A7,  NOTE_G7,  NOTE_E7,  NOTE_C7,  NOTE_D7,   NOTE_B7,  NOTE_F7,  NOTE_C8,  NOTE_A7,  NOTE_G7, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_B7, NOTE_F7, NOTE_C8 }
  };
  int beats[2][16]  = {
    {16, 16, 16,  8,  8,  16, 32, 16, 16, 16, 8, 8, 0, 0, 0, 0 },
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }
  };
*/

int melodies[2][16] = {
  {t_C,  t_b,  t_g,  t_C,  t_b,  t_e,  t_R,  t_C,  t_c,  t_g,  t_a,  t_C,  t_R,  t_R,  t_R,  t_R },
  {NOTE_A7,  NOTE_G7,  NOTE_E7,  NOTE_C7,  NOTE_D7,   NOTE_B7,  NOTE_F7,  NOTE_C8,  NOTE_A7,  NOTE_G7, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_B7, NOTE_F7, NOTE_C8 }
};
int beats[2][16]  = {
  {30, 30, 30,  30,  30,  30, 30, 16, 16, 16, 8, 8, 0, 0, 0, 0 },
  {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }
};
  

  
  unsigned long now = micros();

  // play melody
  if ( melody >=0 && tone_ == 0 ){
    setTone(melodies[melody][note], beats[melody][note] * (unsigned long)10000);
    note++;
    if(note >= 12){
      note = 0;
      melody = -1;
    }
  }

  // play tones
  if ( now < toneBegin + toneDuration && tone_ > 0){
    if ( now - toneChange >= tone_ / 2 ){
      if ( toneState == false){
        digitalWrite(PIN_BUZZER,HIGH);
        toneState = true;
      }else{
        digitalWrite(PIN_BUZZER,LOW);
        toneState = false;
      }
      toneChange = now;
    }
    
  }else{
    tone_ = 0;
    digitalWrite(PIN_BUZZER,LOW);
    toneState = false;
  }
}

void setTone(int myTone, unsigned long duration){
  tone_ = myTone;
  toneDuration = duration;
  toneBegin = micros();
}

void playMelody(int m){
  melody = m;
}

void playMelodyPart(int m, int s, int e){
  melody = m;
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











