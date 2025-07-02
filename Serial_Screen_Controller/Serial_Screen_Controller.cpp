/*
This library provides the serial port interface used by the makerscreen
to communicate with the daughter processor tat controls the LCD in our
dual board Makerscreen carrier. The main idea is that the LCD library 
and code to control the screen is so big that it wouldn't leave much
room for your code to work with, so we put screen stuff all on one board,
and the other uses this lightweight contol library to tell it what to do,
leaving you with plenty of working space.

In this package, we've got a controller object which has several methods
which simply hide all the messy serial communication business from the
main sketch, so that it's not crowded up with screen stuff.
*/

#include "Arduino.h"
#include "Serial_Screen_Controller.h"
#include "SoftwareSerial.h"

//Create the serial connection (pins 3 and 2, to avoid complications on 0 and 1)
SoftwareSerial Ser(3,2);

Serial_Screen_Controller::Serial_Screen_Controller(){
	//Initialize the serial connection
	Ser.begin(9600);
}

void Serial_Screen_Controller::draw_rect(int h, int w, int crn, int dir){
  //Tell the daughter to draw a rectangle: height h, width w, and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision
  Ser.print("&,1,0,"+String(h)+","+String(w)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_rect_fill(int h, int w, int crn, int dir){
  //Tell the daughter to draw a filled rectangle: height h, width w, and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision
  Ser.print("&,3,0,"+String(h)+","+String(w)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_rect_invis(int h, int w, int crn, int dir){
  //Tell the daughter to draw an invisible rectangle: height h, width w, and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision. Invisible rectangles are useful for
  //spacing and moving other objects
  Ser.print("&,12,0,"+String(h)+","+String(w)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_circle(int r, int crn, int dir){
  //Tell the daughter to draw a circle: radius r , and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision
  Ser.print("&,2,0,"+String(r)+","+String(r)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_circle_fill(int r, int crn, int dir){
  //Tell the daughter to draw a filled circle: radius r , and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision
  Ser.print("&,4,0,"+String(r)+","+String(r)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_string(String str, int crn, int dir){
  //Tell the daughter to draw a string, str, and place it
  //in the corner specified by crn, displacing vertically (dir = 0) or
  //horizontally (dir = 1) if there's a collision. Note that strings are
  //placed vertically on lines, and so there is a limited selection of
  //heights that displacements can put them at.
  Ser.print("&,5,0,0,0,"+String(crn)+","+String(dir)+","+str+'*');
  delay(50);
}

void Serial_Screen_Controller::draw_graph(int data[], int len, int h, int M, int crn, int dir){
  //Tell the daughter to draw a graph, of height h, in the corner specified by crn, displacing
  //  according to dir as the other objects. The graph also takes a value M, the max value of the y-axis
  //  for scaling purposes, len, the number of datapoints, and data[], an array containing the data to graph 
  String dout = "";
  for (int i; i < len;i++){
    dout = dout + "," + String(data[i]);}
  Ser.print("&,6,"+String(len)+", "+String(h)+","+String(M)+","+String(crn)+","+String(dir)+dout);
  delay(50);
}

void Serial_Screen_Controller::draw_thermometer(int temp, int crn, int dir){
  //Tell the daughter to draw a thermometer displaying scaled temperature temp, in
  // the corner specified by crn, placed according to dir as the other objects are.
  Ser.print("&,7,0,"+String(temp)+",0,"+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_sun(int crn, int dir){
  //Draw a graphic of a rising sun
  Ser.print("&,8,0,0,0,"+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_sun_full(int crn, int dir){
  //Draw a graphic of a fully risen sun
  Ser.print("&,9,0,0,0,"+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_cloud(int crn, int dir){
  //Draw a graphic of a cloud
  Ser.print("&,10,0,0,0,"+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::draw_clock(int hr, int min, int crn, int dir){
  //Draw a graphic of a clock, displaying hour hr and minute min
  Ser.print("&,11,0,"+String(hr)+","+String(min)+","+String(crn)+","+String(dir));
  delay(50);
}

void Serial_Screen_Controller::clr(){
  //Wipe the screen blank
  Ser.print("&,0,0,0,0,0,0");
  delay(50);
}

void Serial_Screen_Controller::draw(){
  //Update the screen to display the items posted to it so far
  Ser.print("&,999,0,0,0,0,0");
  delay(50);
}
