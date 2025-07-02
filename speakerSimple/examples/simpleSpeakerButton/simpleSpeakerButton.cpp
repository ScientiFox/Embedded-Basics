/*
 *This sketch waits for a button press, then plays a song
 *(a scale, in this case).  
 */

#include <speakerSimple.h>

int speakerPin = 3; //define to which pin the speaker is connected

int buttonPin = 7; //define on which pin the button is connected

int song[] = {//A scale, showing how to make a song
  c4,4,       //  entries in the array alternated between
  d4,4,       //  the note to play and the duration to
  e4,4,       //  play it
  f4,4,
  g4,4,
  a4,4,
  b4,4,
  c5,4,
  -1};

speakerSimple Speaker(speakerPin); //make the speaker object

void setup() {
  //Speaker pin set as output
  pinMode(speakerPin,OUTPUT);

  //Button pin set as input pullup to bias the button
  pinMode(buttonPin,INPUT_PULLUP);
}

void loop() {
  //Hold while the button is not pressed
  while (digitalRead(buttonPin)){}

  //The song gets played when you press the button
  Speaker.play(song);
}



