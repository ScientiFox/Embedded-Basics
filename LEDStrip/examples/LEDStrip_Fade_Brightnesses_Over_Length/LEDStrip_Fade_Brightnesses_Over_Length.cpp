/*
 * Make the LED strip fade up and down along its length
 */
#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(15, 12, 11);

//Hold the value of the anchor pixel
int anchorPixel = 7;

//A list of the brightnesses, values picked for aesthetics
int brightnesses[] = {2,9,16,23,31,37,44,51,44,37,31,23,16,9,2};

void setup() {
}

void loop() {

  for (int i = 0;i<15;i++){
    strip.setPixel((anchorPixel+i)%15,200,brightnesses[i]);
  }

  anchorPixel = (anchorPixel+1)%15;
  
  strip.draw();

  delay(100);
}

