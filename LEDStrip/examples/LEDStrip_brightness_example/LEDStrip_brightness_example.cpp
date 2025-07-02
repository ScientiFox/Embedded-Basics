/*
 * Cycles the brightness of a single pixel
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 12, 11);

/*
 * An incrementor to hold the brightness of the
 * pixel
 */ 
int brightness = 2; //The level of illumination

int brightnessChange = 1; //The rate of change in brightness

void setup() {
}

void loop() { 

  strip.setPixel(strip.ALL,200,brightness); //Set the brightness

  /*
   * Change whether increasing or decreasing brightness
   * if at 0 or 100% level.
   */
  if ((brightness == 0)|(brightness == 100)){
    brightnessChange = (-1)*brightnessChange;
  }

  //Change the brightness level
  brightness = brightness + brightnessChange;

  strip.draw(); //Draw the pixel

  delay(10); //Stability delay
}

