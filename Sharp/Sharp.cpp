/*
A library to read Sharp brand infrared distance sensors

The main contribution is an experimentally derived power law for the distance conversion:
d(inches) ~= 11.77*(voltage)^{-1.22}
*/

#include "Arduino.h"
#include "Sharp.h"

Sharp::Sharp(){
}

//Set the control pin
void Sharp::set_pin(int pin){
	_pin = pin;
}

//Get the distance
float Sharp::get_distance_cm(){
  A_r = analogRead(_pin);
  d = 5.0*(float)(A_r)/1023.0; //5V over 1023 steps scaled
  d = 11.77*pow(d,-1.22); //Empirically derived distance law
  return d;
}

float Sharp::get_distance_inch(){
  //COnvert from inches to cm
  d = get_distance_cm();
  return d/2.54;
}

