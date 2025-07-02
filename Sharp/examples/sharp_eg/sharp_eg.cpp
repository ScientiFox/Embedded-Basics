/*
Nigh-trivial demo for the Sharp library 
(all the work was in finding that conversion function)
*/

//Include lib
#include "Sharp.h"

//Make sensor object
Sharp sharp;

void setup(){
  Serial.begin(9600); //Start serial for reporting
  sharp.set_pin(A0); //Attach sensor to pin A0
}

void loop(){
  Serial.println(sharp.get_distance_cm()); //Report sensor reading in cm
  delay(333); //3Hz update rate
}
