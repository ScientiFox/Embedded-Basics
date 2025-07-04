/*
EPD_lib Durham labs E-Paper Dev Library

Dynamic Graphics on E-Paper with the UNO- Nobody said it could be done!
This libaray implements EPD functions allowing the UNO dynamically control
the PervasiveDisplays E-INK development board.

Currently Included:
-Dynamic Text
-Basic Graphics Objects
	*Circles
	*Lines
	*Squares

Library is limited by 2K SRAM on Arduino, so there is some RLE!
-Chars are mapped dynamically, but to an 8x16 FSM.
	-This is to prevent the issues wwith overhead from mapping onto the RLE
		bitmap- All binary, so interpolation is a huge overhead for little gain.
	-Also, chars are stored as PROGMEM in easily-addressible format- cuts down on
		processor overhead
	-Charmap consumes 128 Bytes of SRAM
-Graphics are processed as elements drawn on a 2x2->1 reduced bitmap
	-Allows dynamic drawing w/o scanning a list of objects (HUGE overhead)
	-Bitmap can be OR'd with charmap lines to produce final output
	-Allows any number of graphics objects w/o bogging down line-by-line
	-Reduced bitmap frees SRAM from graphics, seprating the two allows clean chars
*/

//#includes - necessary parents
#include "Arduino.h"
#include "Durham_EPD.h"
#include <avr/pgmspace.h>

//Saved character set is a literal plopped in memory here
PROGMEM prog_uchar char_set[] = {0x1c,0x3e,0xb8,0x3e,0x7e,0xfe,0xb8,0xee,0x7c,0xf8,0xce,0x1e,0xc3,0xe2,0x38,0x3e,0x38,0x3e,0x78,0x7e,0xee,0xe7,0xe7,0xe7,0xe6,0x38,0x44,0xc4,0x44,0x44,0x84,0xc4,0x44,0x10,0x40,0x44,0x8,0xc6,0x44,0x64,0x44,0x64,0x24,0x64,0x52,0x42,0x42,0x82,0x42,0x44,0x28,0x44,0x82,0x84,0x4,0x84,0x2,0x44,0x10,0x40,0x24,0x8,0xa6,0x48,0x42,0x84,0x42,0x44,0x40,0x52,0x42,0x42,0x5a,0x24,0x24,0x28,0x44,0x2,0x84,0x14,0x24,0x2,0x44,0x10,0x40,0x14,0x8,0xaa,0x48,0x82,0x84,0x82,0x44,0x4,0x12,0x42,0x44,0x5a,0x18,0x18,0x44,0x7c,0x2,0x84,0x1c,0x3c,0x2,0x7c,0x10,0x40,0x1c,0x8,0x9a,0x50,0x82,0x44,0x82,0x3c,0x38,0x10,0x42,0x24,0x5a,0x18,0x18,0x7c,0x84,0x2,0x84,0x4,0x4,0xe2,0x44,0x10,0x42,0x24,0x8,0x92,0x50,0x82,0x3c,0x82,0x24,0x40,0x10,0x42,0x20,0x6a,0x18,0x10,0x44,0x84,0x82,0x84,0x84,0x4,0x82,0x44,0x10,0x42,0x44,0x88,0x82,0x60,0x42,0x4,0x42,0x44,0x42,0x10,0x42,0x8,0x66,0x24,0x10,0x82,0xc4,0x44,0x44,0x84,0x4,0xc4,0x44,0x10,0x24,0x44,0x88,0x82,0x40,0x64,0x4,0x64,0x44,0x46,0x10,0x24,0x18,0x66,0x42,0x10,0xc7,0x3e,0x38,0x3c,0x7e,0x1e,0x38,0xe6,0x7c,0x18,0x8e,0xfe,0xe7,0x4e,0x18,0x1e,0x18,0x8e,0x38,0x3c,0x18,0x10,0x44,0x66,0x3c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x38,0x18,0x38,0x3c,0x20,0x3c,0x70,0x7e,0x38,0x38,0x0,0x0,0x10,0x60,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x14,0x44,0x40,0x30,0x4,0x8,0x40,0x44,0x44,0x10,0x0,0x10,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x10,0x40,0x40,0x28,0x4,0x4,0x20,0x44,0x44,0x10,0x0,0x7c,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x10,0x40,0x30,0x28,0x3c,0x34,0x20,0x24,0x44,0x10,0x0,0x18,0x10,0xfe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x10,0x20,0x60,0x24,0x40,0x4c,0x20,0x3c,0x78,0x7e,0x7e,0x28,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x10,0x10,0x40,0x24,0x40,0x4,0x0,0x44,0x40,0x10,0x0,0x0,0x8,0xfe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x10,0x8,0x40,0x7c,0x40,0x4,0x10,0x44,0x40,0x10,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x48,0x10,0x44,0x64,0x20,0x64,0x48,0x10,0x24,0x20,0x10,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x7c,0x7c,0x18,0x70,0x18,0x30,0x0,0x18,0x1c,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

//Main class
Durham_EPD::Durham_EPD(){
	uint8_t lne[16]; //single 'line' var (row) of bytes
	uint8_t ch_map[8][16]; //char map 16 rows x 8 long of bytes for the EPD itself MMIO
	uint8_t bmp[48][8]; //'bitmap' if needed for external loading
	int _i,_j,_k; //internal incrementors
}

//Wrapper to write a letter
void Durham_EPD::write_letter(int cur_x, int L, int t){
    lne[cur_x] = pgm_read_byte_near(char_set + t*25 + L); //set a byte from charset pointer + table offset plus L index (table is 25 'wide') into lne
    }

//Wrapper to write to the epaper display, at (x,y), some text
void Durham_EPD::write(int cur_x, int cur_y, String text){
  for (_i=0;_i<text.length();_i++){ //loop over all the characters
    ch_map[cur_y][cur_x] = text[_i]-97; //Convert chars from ascii to index, place at X,Y on the display map  
    cur_y = (cur_y + (cur_x==16))*((cur_y + (cur_x==16))< 8); //advance X,Y across the screen in a sweep
    cur_x = (cur_x<16)*(cur_x+1);
  }}

//Function to clear the screen
void Durham_EPD::clear(){    
  for (_j = 0;_j<8;_j++){ //loop over x
    for (_i=0;_i<16;_i++){ //loop over y
     ch_map[_j][_i] = 26; //set map to null
    }}}

//Function to display map onto serial
void Durham_EPD::print_char_map(){
  for (_i = 0;_i<8;_i++){ //Loop over screen map
    for (_j=0;_j<16;_j++){
      if (ch_map[_i][_j] != 26){Serial.print(char(97+ch_map[_i][_j]));} //convert back to ascii
      else{Serial.print("_");}} //print spaces for null chars
    Serial.println();}
}

