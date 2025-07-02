/*
This library provides a clock object which somewhat replaces a real
time clock module. To do this, it provides a Clock object that keeps
a record of the current time as specified by the sketch, and when it 
was last accessed. Every time you ask it for the new time, it checks 
how long it's been since the last check, updates it measure of the time,
and reports that to you. You can get the current hour, minute, and 
second, or a string that is formatted like a normal clock HH:MM:SS.
*/

#include "Arduino.h"
#include "Clock.h"

#define DUR 1000

Clock::Clock(){
	//The clock object carries the time data. Using defaults, because user
	//	will specify time within the program.
	hr = 0; //Hour- 24 hour clock format (easier math)
	hr_12 = 12; //12 hour clock format (prettier display)
	mn = 0; //Current minute
	sec = 0; //Current second
	AMPM = 0; //Mark of whether it's in the AM or the PM
	update(); //Run the update function right away, to get the initial time since check
}

int Clock::getAMPM(){
	//Return whether it's in the AM or the PM
	update(); //Update for the most up to date time
	return AMPM;
}

int Clock::getHour(){
	//Return the current hour, in 12 hr clock format
	update(); //Update for the most up to date time
	return hr_12;
}

int Clock::getMinute(){
	//Return the current minute
	update(); //Update for the most up to date time
	return mn;
}

int Clock::getSecond(){
	//Return the current second
	update(); //Update for the most up to date time
	return sec;
}

String Clock::getTimeString(){
	//Return a standard-format string of the current clock time
	update(); //Update for the most up to date time
	String output = String(hr_12)+":";
	if (mn < 10){
		output = output+"0";}
	output = output+String(mn)+":";
	if (sec < 10){
		output = output+"0";}
	output = output+String(sec) + " ";
	if (AMPM == 0){output = output + "AM";}
	if (AMPM == 1){output = output + "PM";}
	return output;
}

void Clock::setTime(int _hr, int _mn, int _sec, int _AMPM){
	//Set the current time from an external source
	update(); //Update to connect the new readings to the new timestamp
	//Set time parameters, with additional check to handle 24 and 12 hr clocks
	if ((_hr < 24)&(_hr > 12)){
		//Hour from 13 to 24: assume it's 24 hour clock time
		AMPM = 1;
		hr = _hr - 12;
		hr_12 = hr + 12*(hr == 0);
		mn = _mn;
		sec = _sec;}
	else if (_hr >= 24){
		//if greater than 24, who knows? Set it to 12:00:00 AM
		AMPM = 0;
		hr = 0;
		hr_12 = hr + 12*(hr == 0);
		mn = 0;
		sec = 0;
	}
	else{
		//hr is 1-12, 12 hr clock formatting
		AMPM = _AMPM;
		hr = (_hr)*(_hr != 12);
		hr_12 = hr + 12*(hr == 0);
		mn = _mn;
		sec = _sec;}
}

void Clock::update(){
  //This update method counts up hours, minutes, and seconds since the last
  //recorded update via millis(), and uses those counts to update the time
  //dynamically. Unless, millis() overflows (~32 days), it will compute a new
  //time accurately (relative to the user-set time) when called.
  if (millis() - timer > DUR){
  	//Count seconds. DUR is set to allow testing at faster than 1 second/second
    sec = sec + (millis()-timer)/DUR;
    if (sec >= 60){
      //Count up minutes from the seconds
      mn = mn + sec/60;
      sec = sec - 60*(sec/60);} //Remove from seconds count the number of minutes ticked up.
    if (mn >= 60){
      //Count up hours from the minutes
      hr = hr + mn/60;
      mn = mn - 60*(mn/60);} //Remove from minutes the time counted out by the hours
    if (hr >= 12){
	  //Check AM/PM 12 hour rollover
      AMPM = (AMPM + (hr)/12)%2; 
      hr = ((hr)%12); //reset hours to cycled time
    }
    hr_12 = hr + 12*(hr == 0); //Compute te 12 hour clock time from the 24 hour time
    //Reset the time-since-checked by incrementing the counter by 1000 ms for each number of seconds originally counted,
    //  so that the spare < 1s not counted is kept in memory for next time.
    timer = timer + DUR*((millis()-timer)/DUR);} 
}


