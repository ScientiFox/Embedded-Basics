/*
 * This sketch plays a song from the 'songs.h' library,
 * illustrating how to use that library
 */

#include <songs.h>
#include <speakerSimple.h>

int speakerPin = 11; //define to which pin the speaker is connected

speakerSimple Speaker(speakerPin); //make the speaker object

void setup() {
  pinMode(speakerPin,OUTPUT); //Speaker pin
}

void loop() {
  //Playing the 'charge' song from the library
  Speaker.play(charge);

  //Wait one second before looping
  delay(1000);
}



