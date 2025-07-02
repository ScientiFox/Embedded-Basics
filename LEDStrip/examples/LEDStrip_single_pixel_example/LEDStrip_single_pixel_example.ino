/*
  A bouncing pixel on the LED strip
*/

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 12, 11);

int pixel = 7; //A variable to hold the pixel

int Direction = -1; //A variable to hold the direction of travel

int brightness = 10; //Variable to hold the brightness

void setup() {
}

void loop() {
  
  /*
   * Change direction if at either end, and 'flash' the
   * LED by making it brighter.
   */
  pixel = pixel + Direction; //move the pixel
 
  if ((pixel == 0)||(pixel == numPixels-1)){
    Direction = -1*Direction; //Change Direction
    brightness = 100; //'flash' the LED brighter at the end
  }
  else{
    brightness = 10; //the LED is normally kind of dim.
  }
 
  strip.clear(); //Clear the strip
  
  //Set the pixel to the current color, brightness and location
  strip.setPixel(pixel, 299, brightness);

  strip.draw(); //Draw the pixel

  //Stability delay (can be used to slow the sweep speed)
  delay(90 + brightness);

}

