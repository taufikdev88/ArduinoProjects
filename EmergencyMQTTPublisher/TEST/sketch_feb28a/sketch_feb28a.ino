#include <Tone32.h>

#define BUZZER_PIN 4
#define BUZZER_CHANNEL 0

#define BUTTON 34

int step = 0;

void setup() {
  pinMode(BUTTON, INPUT);
}

void loop() {
  if(!digitalRead(BUTTON)){
    tone(BUZZER_PIN, NOTE_F4, 500, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
    tone(BUZZER_PIN, NOTE_C4, 500, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
  }
}
