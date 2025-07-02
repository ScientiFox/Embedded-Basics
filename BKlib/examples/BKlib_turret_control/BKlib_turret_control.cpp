/*
An actual demo script using the BK library
Controls a remote robot with two wheels and a ball-launcher turret
Reads in values from an attached joystick module to move the robot
and buttons to fire the turret
*/

#include <SoftwareSerial.h>
#include <BKlib.h>

//Initialize the soft serial port and connected BK device
SoftwareSerial BKport(2,3);
BKlib BK(&BKport);

//Remote robot's BK module's MAC address
String Node_MAC = "2014,4,143378";

//Joystick variables
int A_v; //Vertical analog reading
int A_h; //Horizontal analog reading
long button_timer = millis(); //Button debounce timer
int button = 0; //Button state

void setup(){
  //prep the native to-pc port and BK soft port
  Serial.begin(9600);
  BKport.begin(38400);

  delay(200); //wait a bit

  BK.initialize(Node_MAC); //Initialize connection to remote module
}

void loop(){
  A_v = analogRead(0); //Read the analog stick values
  A_h = analogRead(1);
  
  //Joystick control is by sending control characters
  //  Sets them by which cardinal direction the stick is at
  // The joystick position can range from 0 to 1000. 500 is about the middle/resting state.

  if ((A_v > 750)&(A_h > 750)){ //Top right sends rightwards diagonal curving
    BKport.write('d');}

  else if ((A_v > 750)&(A_h < 250)){ //Top left sends leftwards diagonal curving
    BKport.write('e');} 
  
  else if ((A_v < 250)&(A_h > 750)){ //Bottom left sends leftwards reverse curving
    BKport.write('h');}
  
  else if ((A_v < 250)&(A_h < 250)){ //Bottom right sends rightwards reverse curving
    BKport.write('g');}
  
  else if (A_v > 750){
    BKport.write('f');}  //When the joystick is far forward, move forward

  else if (A_v < 250){
    BKport.write('b');}    //When the joystick is far back, move in reverse

  else if (A_h > 750){
    BKport.write('l');}    //When the joystick is far right, rotate clockwise

  else if (A_h < 250){
    BKport.write('r');}    //When the joystick is far left, rotate counterclockwise

  else{ //The ranges above leave a middle dead zone
    // When the joystick is towards the middle, send 's' for 'stop'
    BKport.write('s');}
  
  //If the button is pressed and the debounce timer is past its delay limit
  if (!digitalRead(4)&(millis()-button_timer>400)){
    button_timer = millis(); //Reset the timer
    if (button == 0){ //On zero, send turret off command
      BKport.write('k');} 
    if (button == 1){ //On one, send turrent on command
      BKport.write('j');}
    button = 1 - button; //Toggle button state on press
    delay(100); //Wait a sec for smoothness
    while(!digitalRead(4)){ //Wait while off for chatter
      delay(10);}
  }
  
  // A short pause between updates makes the movement more smooth
  delay(100);
}
