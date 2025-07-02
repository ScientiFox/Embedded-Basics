/*
 * The ST7565 library is a package which wraps up the numerous and
 *  difficult to work with elements necessary for interacting with a
 *  graphical LCD into a simple, easy to use interface. The library
 *  contains a number of useful basic functions to simplify communication
 *  with the LCD screen.
 *  
 *  In addition to the useful functions, the library also includes
 *  a built-in mechanism to allow you to place things on the screen
 *  without calculating any coordinates. It does this by letting you
 *  specify a corner, and whether the object you are placing should 
 *  displacy horizontally or vertically when there is a collision.
 */

#include "ST7565.h"

//You have to define these for the library- don't forget them!
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

//Create the object to interact witht the LCD
ST7565 glcd(9, 8, 7, 6, 5); 

void setup() {
  //Initialize the Screen
  glcd.begin(0x01); //Contrast set- 01 is best for our screen
  glcd.clear();  //make sure the display is blank
  glcd.display(); //and draw nothingness

}

void loop() {
  //Draw a string
  glcd.write_string(string,corner, direction);

  //Draw a non-filled rectangle
  glcd.write_rect(height, width,corner,direction);

  //Draw a filled regtangle
  glcd.write_rect_fill(height, width,corner,direction);

  //Draw an empty circle
  glcd.write_circle( r,corner,direction);

  //Draw a filled circle
  glcd.write_circle_fill( r,corner,direction);
  
  //Draw a thermometer- you must supply a temperature -20 to 100
  glcd.write_thermometer( temp, corner,direction);
  
  //Draw a clock- supply the hour (1-12), minute (0-59) and radius
  glcd.write_clock(hour,  min,  r, corner,direction);
  
  /*
   * Draw a graph of data supplied in data[], takes
   * the maximum value expected for the data, and
   * a scaling size
   */
  glcd.write_graph(data[],size, data_max,size, corner, direction);

  /*
   * Draw a bar graph of data supplied in data[], takes
   * the maximum value expected for the data, and
   * a scaling size
   */
  glcd.write_bar_chart( data[],size, data_max,height, corner, direction);

  //Draw a rising sun graphic
  glcd.write_sun(corner, direction);
  
  //Draw an 'invisible' rectangle- used for spacing
  glcd.glcd.write_rect_invis(height, width, corner, direction);
  
  //Draw a cloud graphic
  glcd.write_cloud(corner, direction);
  
  //draw a whole sun
  glcd.write_full_sun(corner, direction);
}
// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense