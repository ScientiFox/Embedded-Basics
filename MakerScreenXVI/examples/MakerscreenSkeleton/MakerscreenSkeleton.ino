/* 
 *  The Makerscreen library makes controlling the 16x2 LED screen
 *  easy, with a collection of simple function to control it.
 */

// include the library code:
#include <MakerScreenXVI.h>

// initialize the library
MakerScreenXVI lcd;

lcd.setup() {
  //Start the LCD
  lcd.begin();
}

lcd.loop() {

  //Clear all text off the LCD screen
  lcd.clear();

  //Return the cursor to the (0,0) position (top left)
  lcd.home();

  //Write some text to the screen
  lcd.write("yourText")

  //Stop displaying text on the screen
  lcd.noDisplay();

  //Resume displaying text that is written to the screen
  lcd.display();

  //Remove the illustration of the cursor on the screen
  lcd.noCursor();

  //Show an illustration of the cursor on the screen
  lcd.cursor();

  //Set the screen to scroll text to the left
  lcd.scrollDisplayLeft();

  //Set the screen to scroll text to the right
  lcd.scrollDisplayRight();

  //Set the screen to automatically scroll text
  lcd.autoscroll();

  //Disable automatic scrolling of text
  lcd.noAutoscroll();

  //Turn on the LCD backlight
  lcd.backlightOn();

  //Turn off the LCD backlight
  lcd.backlightOff();

  //Switch the LCD backlight off if on, and on if off
  lcd.backlightToggle();

  //Display a bargraph on the LCD, plotting value as a proportion
  //  from lowLimit to highLimit
  lcd.bargraph(int value, int lowLimit, int highLimit);

  //Create a custom character, drawn as the binary array below
  lcd.createChar(2,[0b00110,
                    0b10110,
                    0b01100,
                    0b01111,
                    0b01110,
                    0b01011,
                    0b10000,
                    0b00000]
                            );

  //Set the cursot to the location (x,y)
  lcd.setCursor(y,x);

}
