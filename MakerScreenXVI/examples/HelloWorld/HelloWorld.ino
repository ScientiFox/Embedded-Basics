// include the library code:
#include <MakerScreenXVI.h>

// initialize the library with the numbers of the interface pins
MakerScreenXVI lcd;

void setup() {
  // set up the LCD
  lcd.begin();
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

