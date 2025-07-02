/*
 *  A simple example of interacting with the graphic LCD,
 *  drawing a sun icon, and animating it as moving across
 *  the screen using 'invisible' rectangles
 */

#include "ST7565.h"

//Create the ST7565 object to communicate with the screen
ST7565 glcd(9, 8, 7, 6, 5);

//Definitions for the screen
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

void setup(){
  //Initialize the Screen
  glcd.begin(0x01); //Contrast set- 01 is best for our screen

  glcd.clear(); //Blank the screen

  glcd.display(); //Display the blank screen
}

long timer = millis(); //timer- delays interfere with animation

int h = 3; //Width and height of the invisible boxes
int w = 3;

int dw = 2; //change per frame in the invisible boxes
int dh = 1;


void loop(){

  glcd.clear(); //refresh the screen in each frame

  /*
   * Draw two invisible boxes to position the sun- one
   * has fixed width and variable height to set the
   * vertical position, the other fixed height and the
   * other fixed height and variable width to set the
   * horizontal position.
   */
  glcd.write_rect_invis(h,120,2,1);
  glcd.write_rect_invis(10,w,2,0);
  
  /*
   * To better see how these 'invisible' boxes
   * accomplish the positioning, comment the previous two
   * lines and uncomment these:
   */
  //glcd.write_rect(h,120,2,1);
  //glcd.write_rect(10,w,2,0);
  
  /*
   * Draw the sun now that the spacers have shifted
   * the drawing area
   */
  glcd.write_full_sun(2,1);

  glcd.display(); //display everything you just drew  

  /*
   * This function makes the heights and widths of the 
   * spacing boxes above change so that the animation
   * is aesthetically pleasing, the functions are just
   * tweaked untill they look nice
   */
  if (millis()-timer>100){
    h+=dh;
    w+=dw;
    timer = millis();
    if (w > 100){dw = -3;}
    if (w < 3){dw = 3;}
    h = 25 - (abs(w-50))/2;
  }

  delay(50); //Stability delay
}


// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
