/*
 * Generates a color-changing back-and-forth wave on the LED
 * strip, cycling colors through the rainbow as it sweeps. Also
 * changes brightness as you go along.
 * 
 * A good example of the sort of thing you can do with the LED
 * strip, showing changing colors and dynamically addressing 
 * multiple pixels.
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 12, 11);

/*
 * A counter to watch for when to adjust the rates
 * of change of individual colors
 */
int color = 0;

/*
 * An incrementor to hold the position of the 'head'
 * of the colorbar
 */
int pixel = 2;

int Direction = -1; //A variable to hold the direction of travel

int brightness = 100; //A variable to hold the brightenss level

void setup() {
  pinMode(13,INPUT_PULLUP);
}

void loop() { 
  /*
   * Set the head pixel to the current color 
   * color is from -1 to 300, with -1 turning
   * the LED off
   */
  strip.setPixel(pixel, color, brightness);
    
  strip.draw(); //Draw the pixel
  
  delay(1); //Stability delay

  pixel = pixel + Direction; //Increment the head's position

  //Change direction at either end
  if ((pixel == numPixels)||(pixel == 0)){
    Direction = -1*Direction;}

  color+=1; //Increment the color

  brightness+=1; //Increment brightness

  if (brightness == 100){
    brightness = 0;
  }
  
  if (color >= 300){ //Loop the color
    color = 0;
  }
}

