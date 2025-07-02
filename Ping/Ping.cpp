/*
This library packs up the functions for using a PING ultrasonic sensor
into a compact object, so that managing multiple sensors is easier. It
keeps a record of each PING's pin, and includes functions to read the
distance sensors in multiple units.
*/

#include "Arduino.h"
#include "Ping.h"

Ping::Ping(int pin){
	_pin = pin; //The pin the sensor is on
};

long Ping::getUltrasonic(){
  long duration; //The timing variable for reading the pulse length
  pinMode(_pin, OUTPUT); //Set pin as output to toggle
  digitalWrite(_pin, LOW); //Pull the pin low to ready the device
  delayMicroseconds(2); //Pause to let the device register as ready
  digitalWrite(_pin, HIGH); //Pin high to start a reading
  delayMicroseconds(5); //A delay to make sure the start signal is good.
  digitalWrite(_pin, LOW); //Pin pulled low for reading
  pinMode(_pin, INPUT); //Make it an input to check the pulse
  //Read in the pulse, up to 10 ms timeout (about 3m limit- to prevent long delay due to lost pulses, mainly)
  duration = pulseIn(_pin, HIGH,10000); 
  return duration; //Return the bounce time
};

int Ping::getInches(){
	//Get a duration and convert it to inches
	long duration = getUltrasonic();
	return (int)((float)(duration) / 148.0);
};

int Ping::getCentimeters(){
	//Get a duration and convert it to centimeters
	long duration = getUltrasonic();
	return (int)((float)(duration) / 58.0);
};

int Ping::getFeet(){
	//Get a duration and convert it to feet
	long duration = getUltrasonic();
	return (int)((float)(duration) / 148.0 / 12.0);
};

int Ping::getMeters(){
	//Get a duration and convert it to meters
	long duration = getUltrasonic();
	return (float)((duration) / 58.0 / 100.0);
};


