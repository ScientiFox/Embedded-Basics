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
  //Make A0 an output and write HIGH to power the potentiometer
  pinMode(A0,OUTPUT);
  digitalWrite(A0,HIGH);

  //Make A2 an output and write LOW to ground the potentiometer
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);

  //Make A1 an input to read the potentiometer
  pinMode(A1,INPUT);
}

void loop() {

  //Read the potentiometer
  int potentiometer = analogRead(A1);

  //Compute the brightness based on the dial reading
  brightness = map(potentiometer,0,1023,0,100);

  strip.setPixel(strip.ALL,200,brightness); //Set the brightness

  strip.draw(); //Illuminate the pixels

}

