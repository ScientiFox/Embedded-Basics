/*
This library is the backbone of the graphical LCD. It goes with the
daughter board on the graphical LCD carrier board, which uses the
LCD carrier firmware sketch in conjunction with this library to interact
with the screen, receiving commands over a serial port from the mother
board. This library contains all the graphical processing code to draw
the features provided in the serial interface on the screen. The serial
communication is not in this library, so in a case where the code you 
want to run is small enough to fit alongside this large library, you could
also use this package without the serial interface. It doesn't leave much
room, though, so it's not reccommended to do that.

*/

//Necessary libs
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

//Display handler
#include "ST7565.h"
#define ST7565_STARTBYTES 1

//Map of which section is which page. Why is it split half big endian? Who knows, it just is.
const uint8_t pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };

const extern uint8_t PROGMEM font[]; //Display for letter types

//Object mapping and placement tracking
byte objects = 0; //number
byte xs[15]; //position array queues
byte ys[15];
byte ws[15];
byte hs[15];

//Bytes for all the object descriptor holders
byte xm,xM,ym,yM;
byte xms,xMs,yms,yMs;
byte TL,BL,TR,BR;
byte TLs,BLs,TRs,BRs;
byte mx,Mx,my,My;
byte mxs,Mxs,mys,Mys;
byte C,Cs;
byte j,itrs;

// the memory buffer for the LCD
byte st7565_buffer[1024] = { 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x6, 0x0, 0x0, 0x0, 0x3, 0x3, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 


0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 


0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 


0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 


0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,};

//Edges of the update box (for speeding up calculation)
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;

//Function to update the bounding box, which controls where rewrites are.
//	Constraining it reduces computation time /dramatically/
static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
}

//Function to draw a bitmap into the display memory
void ST7565::drawbitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  for (uint8_t j=0; j<h; j++) { //loop over image
    for (uint8_t i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) { //loop over byted
	    my_setpixel(x+i, y+j, color); //set individual pixels
      }
    }
  }
  updateBoundingBox(x, y, x+w, y+h); //always update bound box for draw speed
}

//Function to draw a character array
void ST7565::drawstring(uint8_t x, uint8_t line, char *c) {
  while (c[0] != 0) { //loop over characters
    drawchar(x, line, c[0]); //draw char
    c++; //increment pointer
    x += 6; // 6 pixels wide
    if (x + 6 >= LCDWIDTH) { //plot on screen
      x = 0; //reser pixel ctr
      line++; //advance line
    }
    if (line >= (LCDHEIGHT/8)) //end of off screen
      return;
  }
}

//function to draw a whole string from pointer (basically overloading the above)
void ST7565::drawstring_P(uint8_t x, uint8_t line, const char *str) {
  while (1) {
    char c = pgm_read_byte(str++);
    if (! c)
      return;
    drawchar(x, line, c);
    x += 6; // 6 pixels wide
    if (x + 6 >= LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (LCDHEIGHT/8))
      return;        // ran out of space :(
  }
}

//Function to draw a single character
void  ST7565::drawchar(uint8_t x, uint8_t line, char c) {
  for (uint8_t i =0; i<5; i++ ) {
    st7565_buffer[x + (line*128) ] = pgm_read_byte(font+(c*5)+i); //Grab from the font list index
    x++;
  }
  updateBoundingBox(x, line*8, x+5, line*8 + 8); //Characters have bounding boxes too!
}


//Function to draw a line
//  uses Bresenham's algorithm - thx wikpedia!
void ST7565::drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  uint8_t steep = abs(y1 - y0) > abs(x1 - x0); //correct along least slop step direction
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  // much faster to put the test here, since we've already sorted the points
  updateBoundingBox(x0, y0, x1, y1);

  uint8_t dx, dy; //x and y 'slopes'
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2; //error scale
  int8_t ystep;

  if (y0 < y1) { //stepping y coords in direction
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) { //going across xs
    if (steep) {
      my_setpixel(y0, x0, color); //set pixels based on steepness arg
    } else {
      my_setpixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) { //adjust error to set stepping direction
      y0 += ystep;
      err += dx;
    }
  }
}

// filled rectangle
void ST7565::fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {

  // Simplest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    for (uint8_t j=y; j<y+h; j++) {
      my_setpixel(i, j, color);
    }
  }

  updateBoundingBox(x, y, x+w, y+h);
}

// draw a rectangle
void ST7565::drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {
  // Simplest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    my_setpixel(i, y, color);
    my_setpixel(i, y+h-1, color);
  }
  for (uint8_t i=y; i<y+h; i++) {
    my_setpixel(x, i, color);
    my_setpixel(x+w-1, i, color);
  } 

  updateBoundingBox(x, y, x+w, y+h);
}

// draw a circle outline
void ST7565::drawcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

  // set coordinate values
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  //Set initial quadrant pixels
  my_setpixel(x0, y0+r, color);
  my_setpixel(x0, y0-r, color);
  my_setpixel(x0+r, y0, color);
  my_setpixel(x0-r, y0, color);

  while (x<y) { //over x to y values
    if (f >= 0) { //change y when crossing over
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++; //move x and update crossover value
    ddF_x += 2;
    f += ddF_x;
  
    //set all four circle quadrants' next pixels 
    my_setpixel(x0 + x, y0 + y, color);
    my_setpixel(x0 - x, y0 + y, color);
    my_setpixel(x0 + x, y0 - y, color);
    my_setpixel(x0 - x, y0 - y, color);
    
    my_setpixel(x0 + y, y0 + x, color);
    my_setpixel(x0 - y, y0 + x, color);
    my_setpixel(x0 + y, y0 - x, color);
    my_setpixel(x0 - y, y0 - x, color);    
  }
}

//Draw a hemicircle
//  same as above but draw only 2 quadrants
void ST7565::drawhemicircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  updateBoundingBox(x0-r, y0-r, x0+r, y0);

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  //my_setpixel(x0, y0+r, color);
  my_setpixel(x0, y0-r, color);
  my_setpixel(x0+r, y0, color);
  my_setpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    my_setpixel(x0 + x, y0 - y, color);
    my_setpixel(x0 - x, y0 - y, color);
    
    my_setpixel(x0 + y, y0 - x, color);
    my_setpixel(x0 - y, y0 - x, color);
    
  }
}

//Draw a filled circle
//  same as above, but loop over all pixels between corresponding quadrant lines
void ST7565::fillcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (uint8_t i=y0-r; i<=y0+r; i++) {
    my_setpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    for (uint8_t i=y0-y; i<=y0+y; i++) {
      my_setpixel(x0+x, i, color);
      my_setpixel(x0-x, i, color);
    } 
    for (uint8_t i=y0-x; i<=y0+x; i++) {
      my_setpixel(x0+y, i, color);
      my_setpixel(x0-y, i, color);
    }    
  }
}


// the most basic function, set a single pixel
void ST7565::my_setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7565_buffer[x+ (y/8)*128] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*128] &= ~_BV(7-(y%8)); 
}

//Same overload- naming
void ST7565::setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7565_buffer[x+ (y/8)*128] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*128] &= ~_BV(7-(y%8)); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t ST7565::getpixel(uint8_t x, uint8_t y) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return 0;

  return (st7565_buffer[x+ (y/8)*128] >> (7-(y%8))) & 0x1;  
}

//Activation function for the display
void ST7565::begin(uint8_t contrast) {
  st7565_init();
  st7565_command(CMD_DISPLAY_ON);
  st7565_command(CMD_SET_ALLPTS_NORMAL);
  st7565_set_brightness(contrast);
}

//Initialization function for display
void ST7565::st7565_init(void) {
  // set pin directions
  pinMode(sid, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(cs, OUTPUT);

  // toggle RST low to reset; CS low so it'll listen to us
  if (cs > 0)
    digitalWrite(cs, LOW);

  digitalWrite(rst, LOW);
  _delay_ms(500);
  digitalWrite(rst, HIGH);

  // LCD bias select
  st7565_command(CMD_SET_BIAS_7);
  // ADC select
  st7565_command(CMD_SET_ADC_NORMAL);
  // SHL select
  st7565_command(CMD_SET_COM_NORMAL);
  // Initial display line
  st7565_command(CMD_SET_DISP_START_LINE);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  _delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  st7565_command(CMD_SET_RESISTOR_RATIO | 0x6);

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}

//Wrapper for an SPI write
inline void ST7565::spiwrite(uint8_t c) {
  shiftOut(sid, sclk, MSBFIRST, c);
}

//Wrapper to send a command to the display
void ST7565::st7565_command(uint8_t c) {
  digitalWrite(a0, LOW);
  spiwrite(c);
}

//Wrapper to toggle data loading for display
void ST7565::st7565_data(uint8_t c) {
  digitalWrite(a0, HIGH);
  spiwrite(c);
}

//Wrapper to set screen brightness
void ST7565::st7565_set_brightness(uint8_t val) {
    st7565_command(CMD_SET_VOLUME_FIRST);
    st7565_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}


//Wrapper to display buffer on ST7565
void ST7565::display(void) {
  uint8_t col, maxcol, p; //load value holders

  for(p = 0; p < 8; p++) { //loop over page pointer
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    st7565_command(CMD_SET_PAGE | pagemap[p]); //Send pagemapped buffer


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    //Send bytes for page load
    st7565_command(CMD_SET_COLUMN_LOWER | ((col+ST7565_STARTBYTES) & 0xf));
    st7565_command(CMD_SET_COLUMN_UPPER | (((col+ST7565_STARTBYTES) >> 4) & 0x0F));
    st7565_command(CMD_RMW);
    
    //Send the buffer along
    for(; col <= maxcol; col++) {
      //uart_putw_dec(col);
      //uart_putchar(' ');
      st7565_data(st7565_buffer[(128*p)+col]); //
    }
  }

#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}

// clear everything
void ST7565::clear(void) {
  memset(st7565_buffer, 0, 1024); //set the whole buffer to 0
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1); //free the bounding box
  objects = 0; //No objects left
}


// this doesnt touch the buffer, just clears the display RAM - might be handy
void ST7565::clear_display(void) {
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }     
  }
}


//Function to find the edge of the item boundaries for stacking along horizontal or vertical axes- core of the real time processing
void ST7565::find_box(byte hm,byte wm, byte crn,byte dir){
  //direction- 0 vert, 1-horiz
  //corn- TL TR BL BR: 0 1 2 3
  xm = (0)*(crn%2==0) + (127-wm)*(crn%2==1);
  xM = xm + wm;
  ym = (0)*(crn<2) + (63-hm)*(crn>1);
  yM = ym + hm;
  
  j = 0;  
  while (j<objects){
    //get object extrema - min and max x and y coordinates
    xms = xs[j];
    xMs = xs[j]+ws[j];
    yms = ys[j];
    yMs = ys[j]+hs[j];
    j++;
    
    //sides in ranges? Check  if new object inside old one
    Mx = (xM >= xms)*(xM <= xMs);
    mx = (xm >= xms)*(xm <= xMs);
    My = (yM >= yms)*(yM <= yMs);
    my = (ym >= yms)*(ym <= yMs);
    
    //corners inside box?
    TL = mx*my;
    TR = Mx*my;
    BL = mx*My;
    BR = Mx*My;
    
    //sides in ranges? Check  if old object inside new one
    Mxs = (xMs >= xm)*(xMs <= xM);
    mxs = (xms >= xm)*(xms <= xM);
    Mys = (yMs >= ym)*(yMs <= yM);
    mys = (yms >= ym)*(yms <= yM);
    
    //corners inside box?
    TLs = mxs*mys;
    TRs = Mxs*mys;
    BLs = mxs*Mys;
    BRs = Mxs*Mys;
	
	Cs = (Mxs*mxs)*(My*my);
	C  = (Mx*mx)*(Mys*mys);
    
    //Final variable which indicates whether or not tere is an intersection
    itrs = (TL+TR+BL+BR+TLs+TRs+BLs+BRs+C+Cs)>0;
    
    //depending on direction of travel
    //direction- 0 vertical, 1-horizontal
    switch (crn){
      case (0):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym + itrs*(1-dir)*(yMs -ym + 1);
        break;
      case (1):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym + itrs*(1-dir)*(yMs - ym + 1);
        break;
      case (2):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym - itrs*(1-dir)*(yM - yms + 1);
        break;
      case (3):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym - itrs*(1-dir)*(yM - yms + 1);
        break;
    }
    
    //Increment up over width and height steps
    xM = xm + wm;
    yM = ym + hm;
    
    //Check full pixel boundaries and correct overruns
    if (xM > 128){
      xm = 0;
      xM = wm;
      ym = ym + (1)*(crn < 2) + (-1)*(crn > 1);
      yM = ym + hm;}
    
    if (xm < 0){
      xm = 127 - wm;
      xM = 127;
      ym = ym + (1)*(crn < 2) + (-1)*(crn > 1);
      yM = ym + hm;}
    
    if (yM > 64){
      ym = 0;
      yM = ym + hm;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
    
    if (ym < 0){
      ym = 63 - hm;
      yM = 63;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
      
    if (itrs > 0){
      j = 0;}
  }

  //Set final bound positions  
  xs[objects] = xm;
  ys[objects] = ym;
  ws[objects] = xM-xm;
  hs[objects] = yM-ym;
  objects+=1;
  //drawrect(xm,ym,xM-xm,yM-ym,1); //Optional, to illustrate the location of the empty slot
    
}

//FInd the box of a whole string, not just individual characters
void ST7565::find_box_string(byte hm,byte wm, byte crn,byte dir){
  //direction- 0 vert, 1-horiz
  //corn- TL TR BL BR: 0 1 2 3
  xm = (0)*(crn%2==0) + (127-wm)*(crn%2==1);
  xM = xm + wm;
  ym = (0)*(crn<2) + (63-hm)*(crn>1);
  yM = ym + hm;
  
  j = 0;  
  while (j<objects){
    //get object
    xms = xs[j];
    xMs = xs[j]+ws[j];
    yms = ys[j];
    yMs = ys[j]+hs[j];
    j++;
    
    //sides in ranges?
    Mx = (xM >= xms)*(xM <= xMs);
    mx = (xm >= xms)*(xm <= xMs);
    My = (yM >= yms)*(yM <= yMs);
    my = (ym >= yms)*(ym <= yMs);
    
    //corners inside box?
    TL = mx*my;
    TR = Mx*my;
    BL = mx*My;
    BR = Mx*My;
    
    //sides in ranges? Check  if old object inside new one
    Mxs = (xMs >= xm)*(xMs <= xM);
    mxs = (xms >= xm)*(xms <= xM);
    Mys = (yMs >= ym)*(yMs <= yM);
    mys = (yms >= ym)*(yms <= yM);
    
    //corners inside box?
    TLs = mxs*mys;
    TRs = Mxs*mys;
    BLs = mxs*Mys;
    BRs = Mxs*Mys;
	
	Cs = (Mxs*mxs)*(My*my);
	C  = (Mx*mx)*(Mys*mys);
    
    //Final variable which indicates whether or not tere is an intersection
    itrs = (TL+TR+BL+BR+TLs+TRs+BLs+BRs+C+Cs)>0;
    
    //direction- 0 vert, 1-horiz
    switch (crn){
      case (0):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym + itrs*8*(1-dir);
        break;
      case (1):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym + itrs*8*(1-dir);
        break;
      case (2):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym - itrs*8*(1-dir);
        break;
      case (3):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym - itrs*8*(1-dir);
        break;
    }
    
    xM = xm + wm;
    yM = ym + hm;

    //Check full pixel boundaries and correct overruns
    if (xM > 128){
      xm = 0;
      xM = wm;
      ym = ym + (8)*(crn < 2) + (-8)*(crn > 1);
      yM = ym + hm;}
    
    else if (xm < 0){
      xm = 127 - wm;
      xM = 127;
      ym = ym + (8)*(crn < 2) + (-8)*(crn > 1);
      yM = ym + hm;}
    
    else if (yM > 64){
      ym = 0;
      yM = ym + hm;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
    
    else if (ym < 0){
      ym = 64 - hm;
      yM = 64;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
      
    if (itrs > 0){
      j = 0;}
  }
  
  xs[objects] = xm;
  ys[objects] = ym;
  ws[objects] = xM-xm;
  hs[objects] = yM-ym;
  objects+=1;
  //drawrect(xm,ym,xM-xm,yM-ym,1);
    
}

//Function to find a box that fits at all, regardless of stacking
void ST7565::find_box_anywhere(byte hm,byte wm, byte x0, byte y0, byte crn, byte dir){
  xm = x0;
  xM = xm + wm;
  ym = y0;
  yM = ym + hm;
  
  j = 0;  
  while (j<objects){
    //get object extrema - min and max x and y coordinates
    xms = xs[j];
    xMs = xs[j]+ws[j];
    yms = ys[j];
    yMs = ys[j]+hs[j];
    j++;
    
    //sides in ranges? Check  if new object inside old one
    Mx = (xM >= xms)*(xM <= xMs);
    mx = (xm >= xms)*(xm <= xMs);
    My = (yM >= yms)*(yM <= yMs);
    my = (ym >= yms)*(ym <= yMs);
    
    //corners inside box?
    TL = mx*my;
    TR = Mx*my;
    BL = mx*My;
    BR = Mx*My;
    
    //sides in ranges? Check  if old object inside new one
    Mxs = (xMs >= xm)*(xMs <= xM);
    mxs = (xms >= xm)*(xms <= xM);
    Mys = (yMs >= ym)*(yMs <= yM);
    mys = (yms >= ym)*(yms <= yM);
    
    //corners inside box?
    TLs = mxs*mys;
    TRs = Mxs*mys;
    BLs = mxs*Mys;
    BRs = Mxs*Mys;
	
	Cs = (Mxs*mxs)*(My*my);
	C  = (Mx*mx)*(Mys*mys);
    
    //Final variable which indicates whether or not there is an intersection
    itrs = (TL+TR+BL+BR+TLs+TRs+BLs+BRs+C+Cs)>0;
    
    //depending on direction of travel
    //direction- 0 vertical, 1-horizontal
    switch (crn){
      case (0):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym + itrs*(1-dir)*(yMs -ym + 1);
        break;
      case (1):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym + itrs*(1-dir)*(yMs - ym + 1);
        break;
      case (2):
        xm = xm + itrs*(dir)*(xMs - xm + 1);
        ym = ym - itrs*(1-dir)*(yM - yms + 1);
        break;
      case (3):
        xm = xm - itrs*(dir)*(xM - xms + 1);
        ym = ym - itrs*(1-dir)*(yM - yms + 1);
        break;
    }
    
    xM = xm + wm;
    yM = ym + hm;
    
    //Check full pixel boundaries and correct overruns
    if (xM > 128){
      xm = 0;
      xM = wm;
      ym = ym + (1)*(crn < 2) + (-1)*(crn > 1);
      yM = ym + hm;}
    
    if (xm < 0){
      xm = 127 - wm;
      xM = 127;
      ym = ym + (1)*(crn < 2) + (-1)*(crn > 1);
      yM = ym + hm;}
    
    if (yM > 64){
      ym = 0;
      yM = ym + hm;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
    
    if (ym < 0){
      ym = 63 - hm;
      yM = 63;
      xm = xm + (1)*(crn%2==0) + (-1)*(crn%2==1);
      xM = xm + wm;}
      
    if (itrs > 0){
      j = 0;}
  }
  
  xs[objects] = xm;
  ys[objects] = ym;
  ws[objects] = xM-xm;
  hs[objects] = yM-ym;
  objects+=1;
  //drawrect(xm,ym,xM-xm,yM-ym,1);
    
}

//Wrapper to draw a circle
void ST7565::write_circle(byte r, byte crn,byte dir){
  find_box(2*r+1,2*r+1,crn,dir);
  drawcircle(xm + r,ym + r, r, 1); 
}

//Wrapper to draw a filled circle
void ST7565::write_circle_fill(byte r, byte crn,byte dir){
  find_box(2*r+1,2*r+1,crn,dir);
  fillcircle(xm + r,ym + r, r, 1); 
}

//Wrapper to draw a rectangle
void ST7565::write_rect(byte h, byte w, byte crn,byte dir){
  find_box(h,w,crn,dir);
  drawrect(xm,ym,w,h,1);
}

//Wrapper to draw a filled rectangle
void ST7565::write_rect_fill(byte h, byte w, byte crn, byte dir){
  find_box(h,w,crn,dir);
  fillrect(xm,ym,w,h,1);
}

//Wrapper to draw a space-filling invisible rectangle
void ST7565::write_rect_invis(byte h, byte w, byte crn, byte dir){
  find_box(h,w,crn,dir);
  fillrect(xm,ym,w,h,0);
}

//Wrapper to draw an isosceles triangle
void ST7565::write_triangle_isosceles(byte h, byte w, byte crn, byte dir){
	find_box(h,w,crn,dir);
	drawline(xm+(w/2), ym , xm+w, ym+h, 1); //L12
	drawline(xm+w, ym+h, xm, ym+h, 1); //L23
	drawline(xm, ym+h, xm+(w/2), ym , 1); //L31
}

//Wrapper to draw a string
void ST7565::write_string(String string, byte crn, byte dir){
  char *c = &string[0];
  byte i = 0;
  char *cp = c; 
  while (cp[0] != 0){
    cp++;
    i++;}
  find_box_string(8,6*i+1,crn,dir);

  //glcd.drawrect(xm,ym,xM-xm,yM-ym,1);
  drawstring(xm,(ym+1)/8,c);
}

//Wrapper to draw a thermometer display object
void ST7565::write_thermometer(byte temp, byte crn, byte dir){
	find_box(30,27,crn,dir);
	fillcircle(xm+13,ym+23,7,1);
	byte ht = (byte)((((float)(temp)+20)/120.0)*20.0);
	fillrect(xm+9,ym+(23-ht),8,ht,1);
	fillcircle(xm+13,ym+(23-ht),3,1);
	for (byte i=ym+(23-ht)+(ht%2==1);i<ym+20;i+=2){
		drawline(xm+9,i,xm+11,i,0);}
	//drawrect(xm,ym,26,31,1);
}

//Wrapper to draw a clock object
void ST7565::write_clock(byte hr, byte min, byte r, byte crn, byte dir){
	float th = 180 - hr*30;
	float tm = 180 - min*6;
	
	if (r<6){r = 6;}
	
	byte cxh = (byte)((float)(r-5)*sin(th*3.142/180.0));
	byte cyh = (byte)((float)(r-5)*cos(th*3.142/180.0));
	byte cxm = (byte)((float)(r-2)*sin(tm*3.142/180.0));
	byte cym = (byte)((float)(r-2)*cos(tm*3.142/180.0));
	
	find_box(2*r+1,2*r+1,crn,dir);
	drawcircle(xm+r,ym+r,10,1);
	drawline(xm+r,ym+r,xm+r+cxh,ym+r+cyh,1);
	drawline(xm+r,ym+r,xm+r+cxm,ym+r+cym,1);
}

//Wrapper to draw a data plot object
void ST7565::write_graph(int data[],byte size,byte data_max,byte h, byte crn, byte dir){
	byte d = ((h/size)+1);
	byte w = d*(size+1);
	find_box(h+2,w,crn,dir);
	drawrect(xm,ym,w,h+2,1);

	byte yd;
	byte yp = (byte)(((float)(data[0]))*(h)/(data_max));
	for (byte i=1;i<size;i++){
		yd = (byte)(((float)(data[i]))*(h)/(data_max));
		drawline(xm+d*i,ym+(h-yp),xm+d*(i+1),ym+(h-yd),1);
		yp = yd;}
}

//Wrapper to draw a bar chart object
void ST7565::write_bar_chart(byte data[],byte size,byte data_max,byte h, byte crn, byte dir){
	byte d = ((h/size)+1);
	byte w = d*(size+1);
	find_box(h+2,w,crn,dir);
	drawrect(xm,ym,w,h+2,1);

	byte yd;
	byte yp = (byte)(((float)(data[0]))*(h)/(data_max));
	for (byte i=1;i<size;i++){
		yd = (byte)(((float)(data[i]))*(h)/(data_max));
		fillrect(xm+d*i,ym+(h-yd),d,yd,1);}
}

//Wrapper to draw a sun
void ST7565::write_sun(byte crn, byte dir){
	//r1, r2, and r3 tell the sizes of the snowman
	//crn tells what corner to go to
	//dir tells it whether to mov horizontally or vertically to evade collisions
	byte h = 20;
	byte w = 40;

	//place the bounding box
	find_box(h,w,crn,dir);
	
	//circle
	drawhemicircle(xm+4*5,ym+4*5,10,1);
	drawline(xm+2*5,ym+4*5,xm+6*5,ym+4*5,1);
	
	//triangle
	drawline(xm+0*5, ym+4*5 , xm+1*5, ym+3*5, 1); //L12
	drawline(xm+1*5, ym+3*5, xm+1*5, ym+4*5, 1); //L23
	drawline(xm+1*5, ym+4*5, xm+0*5, ym+4*5 , 1); //L31

	//triangle
	drawline(xm+1*5+2, ym+2*5+2, xm+1*5+2, ym+1*5+2, 1); //L12
	drawline(xm+1*5+2, ym+1*5+2, xm+2*5+2, ym+1*5+2, 1); //L23
	drawline(xm+2*5+2, ym+1*5+2, xm+1*5+2, ym+2*5+2, 1); //L31

	//triangle
	drawline(xm+3*5+1, ym+1*5+1, xm+4*5, ym+0*5+2, 1); //L12
	drawline(xm+4*5, ym+0*5+2, xm+5*5-1, ym+1*5+1, 1); //L23
	drawline(xm+5*5-1, ym+1*5+1, xm+3*5+1, ym+1*5+1, 1); //L31

	//triangle
	drawline(xm+6*5-2, ym+1*5+2, xm+7*5-2, ym+1*5+2, 1); //L12
	drawline(xm+7*5-2, ym+1*5+2, xm+7*5-2, ym+2*5+2, 1); //L23
	drawline(xm+7*5-2, ym+2*5+2, xm+6*5-2, ym+1*5+2, 1); //L31
	
	//triangle
	drawline(xm+7*5, ym+3*5, xm+7*5, ym+4*5, 1); //L12
	drawline(xm+7*5, ym+4*5, xm+8*5, ym+4*5, 1); //L23
	drawline(xm+8*5, ym+4*5, xm+7*5, ym+3*5, 1); //L31
}

//Wrapper to draw a cloud
void ST7565::write_cloud(byte crn, byte dir){
	byte h = 20;
	byte w = 40;

	//place the bounding box
	find_box(h,w,crn,dir);
	//drawrect(xm,ym,w,h,1);
	
	drawcircle(xm+7,ym+7,7,1);
	fillcircle(xm+7,ym+7,6,0);
	
	drawcircle(xm+17,ym+8,7,1);
	fillcircle(xm+17,ym+8,6,0);
	
	drawcircle(xm+28,ym+9,7,1);
	fillcircle(xm+28,ym+9,6,0);
	
	drawcircle(xm+22,ym+11,7,1);
	fillcircle(xm+22,ym+11,6,0);
	
	drawcircle(xm+14,ym+12,7,1);
	fillcircle(xm+14,ym+12,6,0);
	
	fillrect(xm+7,ym+5,22,8,0);
	
	drawhemicircle(xm+17,ym+13,7,1);
	fillrect(xm+20,ym+11,6,4,0);
	fillrect(xm+9,ym+11,4,4,0);
	
	fillrect(xm+28,ym+7,4,4,0);
}

//Wrapper to draw a triangle
void ST7565::draw_triangle(byte xm, byte ym, byte x1, byte y1, byte x2, byte y2, byte x3, byte y3){
	drawline(xm+x1, ym+y1, xm+x2, ym+y2, 1); //L12
	drawline(xm+x2, ym+y2, xm+x3, ym+y3, 1); //L23
	drawline(xm+x3, ym+y3, xm+x1, ym+y1, 1); //L31
}

//Wrapper to draw a full mapped sun object
void ST7565::write_full_sun(byte crn, byte dir){
	byte h = 23;
	byte w = 23;
	find_box(h,w,crn,dir);
	
	//circle
	drawcircle(xm+11,ym+11,5,1);
	//drawrect(xm,ym,24,24,1);

	int offset = 4;

	//triangles
	draw_triangle(xm-offset, ym-offset,8,8,11,9,9,11);
	draw_triangle(xm-offset, ym-offset,15,5,16,8,14,8);
	draw_triangle(xm-offset, ym-offset,19,9,22,8,21,11);
	draw_triangle(xm-offset, ym-offset,25,15,22,14,22,16);
	draw_triangle(xm-offset, ym-offset,22,22,19,21,21,19);
	draw_triangle(xm-offset, ym-offset,15,25,14,22,16,22);
	draw_triangle(xm-offset, ym-offset,8,22,11,21,9,19);
	draw_triangle(xm-offset, ym-offset,5,15,8,14,8,16);
	
}
