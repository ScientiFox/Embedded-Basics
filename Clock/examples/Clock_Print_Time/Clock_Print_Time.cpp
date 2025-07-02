/*
 * A very basic use of the clock library- keeping time from an
 * assumed point of 12 AM sharp, printed to the serial port.
 */
#include "Clock.h"

//Make an object to be the clock
Clock clock = Clock();

void setup() {
  //Start the serial port
  Serial.begin(9600);
}

void loop() {
  //Get the current time according to the clock
  String timeNow = clock.getTimeString();

  //Print the current time
  Serial.println(timeNow);

  //Wait a little while
  delay(800);
}

