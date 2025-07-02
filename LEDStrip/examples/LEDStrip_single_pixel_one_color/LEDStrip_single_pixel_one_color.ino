/*
 * Illuminates a single pixel with a color.
 * This version uses the 1-D color conversion:
 *   I
 *   |b   r    g    b
 *   |\   /\   /\   /
 *   | \ /  \ /  \ /
 *   |  X    X    X
 *   | / \  / \  / \
 *   |/___\/___\/___\__
 *   0    100  200  300
 * So that mix_color_int() takes an integer 'color' from
 * 0 to 300, and an array {b,g,r} to store the colors.
 */

#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(15, 12, 11);

void setup() {
}

void loop() {
  strip.setPixel(7, 35); //Set the pixel to a given color

  strip.draw(); //Draw the pixel

}
