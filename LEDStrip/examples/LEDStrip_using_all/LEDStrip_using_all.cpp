/*
 *Shows the use of the all() function to illuminate
 *all pixels on a strip at once
 */

#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(15, 12, 11);

/*
 * A counter to watch for when to adjust the rates 
 * of change of individual colors
 */
int color = 0;

void setup() {
}

void loop() {

  strip.setPixel(strip.ALL,color,50); //Set all pixels to the current color

  strip.draw(); //Draw pixels

  delay(20); //Stability delay

  color++; //Increment the color

  if (color == 300){ //Loop the color
    color = 0;}
}

