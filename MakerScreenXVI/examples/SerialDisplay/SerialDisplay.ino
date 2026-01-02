 
//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Make an LCD display object, these numbers are the pins that the
//  Maker Screen uses
MakerScreenXVI lcd;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin();
  
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}
