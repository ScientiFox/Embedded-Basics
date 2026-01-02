//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Make an LCD display object, these numbers are the pins that the
//  Maker Screen uses
MakerScreenXVI lcd;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin();
}

void loop() {
  // loop from ASCII 'a' to ASCII 'z':
  for (int thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
    // loop over the columns:
    for (int  thisRow = 0; thisRow < 2; thisRow++) {
      // loop over the rows:
      for (int thisCol = 0; thisCol < 16; thisCol++) {
        // set the cursor position:
        lcd.setCursor(thisCol, thisRow);
        // print the letter:
        lcd.write(thisLetter);
        delay(200);
      }
    }
  }
}


