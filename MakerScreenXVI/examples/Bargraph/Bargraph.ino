/*
 * Display on an LCD screen a bar graph of the value read in from
 *  a turn knob
 */
 
//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Make an LCD display object, these numbers are the pins that the
//  Maker Screen uses
MakerScreenXVI lcd;

int columns = -1; //The number of bars to fill on the display

void setup() {
  
  //Initialize the LCD
  lcd.begin(); //Columns, Rows

  lcd.clear(); //Make sure the display is empty

  pinMode(A0,OUTPUT);
  digitalWrite(A0,HIGH);  //Either turn knob outer pin
  pinMode(A1,INPUT);      //Middle pin of turn knob
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);   //Either turn knob outer pin

  }

void loop() {
  int knobPosition = analogRead(A1); //Get the position of the Knob
  
  //Print the measured analog value on line 0
  lcd.setCursor(0,0); //Set cursor to upper left
  lcd.print(knobPosition);
  //Erase any characters still there after the number
  lcd.print("    "); 

  lcd.bargraph(knobPosition,0,1023);
  
  delay(100); //Delay to reduce flicker
}

//Replace the 255 in lcd.write with a number or variable to change 
//  the appearance of the blocks

//(c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

