#ifndef Clock_lib
#define Clock_lib

#include "Arduino.h"

class Clock{
  public:
  	Clock();
  	void update();
  	int getAMPM();
  	int getHour();
  	int getMinute();
  	int getSecond();
  	void setTime(int _hr, int _mn, int _sec, int _AMPM);
  	String getTimeString();
  	int AM;
  	int PM;
  private:
  	int AMPM;
  	int hr;
  	int mn;
  	int sec;
  	int hr_12;
  	long timer;
};

#endif
