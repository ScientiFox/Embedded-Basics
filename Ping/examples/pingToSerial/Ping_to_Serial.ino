/*
Demo using the ping library to report results to serial
*/

//Load the lib
#include "Ping.h"

//Make a ping object
Ping distance(12);

void setup(){
  Serial.begin(9600); //Start serial for reporting
}

void loop(){
  Serial.println(distance.getInches()); //Send measured distance in inches to port
}
