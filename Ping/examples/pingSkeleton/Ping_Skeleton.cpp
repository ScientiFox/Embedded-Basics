/*
  The Ping library is a utility package that maks interfacing with
  an ultrasonic distance sensor very simple and straight-forward.
  It contains a short list of functions which handle all timing 
  and polling of the sensor, and return a converted distance measurement.
*/

//Include lib
#include <Ping.h>

//Make a sensor
Ping distanceSensor(3);

void setup() {
}

void loop() {
  //Get readings from the sensor
  distanceSensor.getUltrasonic(); //Raw uS timing
  distanceSensor.getInches(); //convert to inches
  distanceSensor.getCentimeters(); //convert to centimeters
  distanceSensor.getFeet(); //convert to feet
  distanceSensor.getMeters(); //convert to meters
}
