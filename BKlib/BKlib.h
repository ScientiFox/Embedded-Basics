#ifndef BKlib_h
#define BKlib_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class BKlib{
	public:
		BKlib(SoftwareSerial* port);
		void initialize(String MAC);
		void call_and_response(String command);
		void get_addr();
		void set_hub();
		void set_node();
		void add_link(String MAC);
	private:
		SoftwareSerial* _port;
};


#endif
