//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Make an LCD display object, these numbers are the pins that the
//  Maker Screen uses
MakerScreenXVI lcd;


void setup() {
  //Initialize the LCD
  lcd.begin();

  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // Turn off the cursor:
  lcd.noCursor();
  delay(500);
  // Turn on the cursor:
  lcd.cursor();
  delay(500);
}

