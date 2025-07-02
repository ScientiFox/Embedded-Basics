/*
 * Makes the brightness of all pixels inversely proportionate
 * to the reading on a photosensor
 */
#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(15, 12, 11);

int brightness = 2; //The level of illumination

void setup() {
  //Make A0 an inputtlup to power the photosensor
  pinMode(A0,INPUT_PULLUP);
}

void loop() {

  //Read the censor
  int photoSensor = analogRead(A0);

  //Set limits on sensor value
  photoSensor = constrain(photoSensor,100,900);

  //Compute the brightness based on the photosensor
  brightness = map(photoSensor,100,900,0,100);

  strip.setPixel(strip.ALL,200,brightness); //Set the brightness

  strip.draw(); //Illuminate the pixels

}
