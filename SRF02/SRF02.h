#ifndef srf02h
#define srf02h

#include "Arduino.h"

class SRF02{
	public:
		SRF02(int address, int channel);
		bool start_measure();
		bool read_distance();
		
		bool started;
		
		int addr;
		int wire_channel;
		int reading;
		long timer;
		
		

};

#endif


