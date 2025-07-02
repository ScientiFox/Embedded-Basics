/*
Durham_EPD header file
*/

#ifndef Durham_EPD_h
#define Durham_EPD_h

#include "Arduino.h"

class Durham_EPD{
	public:
		Durham_EPD();
		void write(int cur_x, int cur_y, String text);
		void clear();
		void print_char_map();
		uint8_t lne[16];
		uint8_t ch_map[8][16];
		uint8_t bmp[48][8];
		void write_letter(int cur_x, int L, int t);	

	private:
		int _i,_j,_k;	
};

#endif
