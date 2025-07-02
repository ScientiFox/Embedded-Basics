/*
 * Makes the brightness of all pixels proportionate
 * to the reading on a microphone
 */
#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(15, 12, 11);

void setup() {
  pinMode(A5,OUTPUT); //Mircophone Power
  digitalWrite(A5,HIGH);

  pinMode(A4,INPUT); //Microphone input

  pinMode(A3,OUTPUT); //Microphone ground
  digitalWrite(A3,LOW);

}

void loop() {

  //Read the sensor on the analog port:
  int soundValue = analogRead(A4);

  //Compute the brightness based on the photosensor
  int brightness = map(soundValue,0,1023,0,100);

  strip.setPixel(strip.ALL,200,brightness); //Set the brightness

  strip.draw(); //Illuminate the pixels

}

