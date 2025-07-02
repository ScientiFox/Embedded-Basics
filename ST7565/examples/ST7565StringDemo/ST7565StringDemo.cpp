/*
 * A simple example of interacting with the graphic LCD,
 * writing a string of a timer count.
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

  glcd.clear(); //Clear the screen

  glcd.display(); //Display the blank screen
}

long timer = millis(); //timer- delays interfere with animation

int counter = 0; //The counter to display

void loop(){

  //refresh the screen in each frame
  glcd.clear();

  //Draw the string with the count
  glcd.write_string(String(counter)+" Ticks",2,1);

  glcd.display(); //display what you just drew  
  
  if (millis()-timer>100){ //increment the counter every 100 mS  
    counter+=1;
  }

  delay(50); //Stability delay
}


