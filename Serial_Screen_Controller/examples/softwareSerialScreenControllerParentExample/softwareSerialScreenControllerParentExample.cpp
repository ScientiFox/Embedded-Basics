/*
 * Example illustrating the use of the Serial Screen controller
 */

#include "SoftwareSerial.h"
#include "Serial_Screen_Controller.h"

//Create screen controller object
Serial_Screen_Controller screen;

void setup(){
}

void loop(){
  screen.clr(); //Clear the screen each time

  //read in a value from the analog port
  int val = analogRead(A0);

  //draw a string to the screen
  screen.draw_string("V: "+String(val),4+(val>9)+(val>99)+(val>999),TOP_RIGHT,HORIZONTAL);

  //display a graph of the analog data
  screen.draw_graph(data,5,20,1025,BOTTOM_LEFT,HORIZONTAL);

  /*
   * draw a circle- its radius is changing- you can dynamically
   * adjust objects
   */
  screen.draw_circle_fill(r-2,BOTTOM_LEFT,HORIZONTAL);

  screen.draw(); //display all the drawn elements on the screen

  /*
   * Update the chart data- scrolling across, so shift
   * everything over and add the new datapoint at the end
   */
  for (int i = 5;i>0;i--){
    data[i] = data[i-1];}
  data[0] = random(0,18);

  /*
   *Just a weird function to grow the radius of the
   *circle over time 
   */
  r =  (r+1)*(r < 20) + 6*(r == 20);
  
  delay(10); //Stability delay
}


