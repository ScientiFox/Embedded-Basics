/*
 * Skeleton for the Clock Library, illustrating all functions.
 * 
 * Updates based on dividing up millis() checks, and you don't have
 * to call update() with a given frequency.
 * 
 * You can call clock.update() any time to update it, but all
 * functions force an update when you call them, so you can ignore
 * update() totally if you want.
 * 
 */
#include "Clock.h"

//Make an object to be the clock
Clock clock;

void setup() {
  //Initialize the time to hour:minute:second AM
  clock.setTime(hour,minute,second,clock.AM);
}

void loop() {
  //Get the current time according to the clock
  String timeNow = clock.getTimeString();

  //Make the clock update from last measurement
  clock.update();

  //Get whether in the AM (0) or PM (1)
  int AMPM = getAMPM();

  //Get the current hour (1-12)
  int hour = getHour();

  //Get the current minute
  int minute = getMinute();

  //Get the current second
  int second = getSecond();
}
