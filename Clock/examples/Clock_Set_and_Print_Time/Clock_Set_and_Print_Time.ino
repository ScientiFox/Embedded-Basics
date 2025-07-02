/*
 * Using the Clock library to keep time, set in setup(), and 
 * printing to the serial port.
 */
#include "Clock.h"

//Make an object to be the clock
Clock clock;

void setup() {
  //Start the serial port
  Serial.begin(9600);

  //Initialize the time to 2:31 PM
  clock.setTime(2,31,00,clock.PM);
}

void loop() {
  //Get the current time according to the clock
  String timeNow = clock.getTimeString();

  //Print the current time
  Serial.println(timeNow);

  //Wait a little while
  delay(800);

}

