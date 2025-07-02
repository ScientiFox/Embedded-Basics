/*
EPD library example showing real-time updates with a bouncing ball on screen!
*/

#include <inttypes.h>
#include <ctype.h>

// required libraries
#include <SPI.h>
#include <FLASH.h>
#include <EPD.h>
#include <S5813A.h>

//Setup for EPD library
const int Pin_TEMPERATURE = A0;
const int Pin_PANEL_ON = 2;
const int Pin_BORDER = 3;
const int Pin_DISCHARGE = 4;
const int Pin_PWM = 5;
const int Pin_RESET = 6;
const int Pin_BUSY = 7;
const int Pin_EPD_CS = 8;
const int Pin_FLASH_CS = 9;
const int Pin_SW2 = 12;
const int Pin_RED_LED = 13;

// LED anode through resistor to I/O pin
// LED cathode to Ground
#define LED_ON  HIGH
#define LED_OFF LOW

//EPD size setting
#define EPD_SIZE EPD_1_44


// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS);


// I/O setup
void setup() {
	//Prep the control pins
	pinMode(Pin_RED_LED, OUTPUT);
	pinMode(Pin_SW2, INPUT);
	pinMode(Pin_TEMPERATURE, INPUT);
	pinMode(Pin_PWM, OUTPUT);
	pinMode(Pin_BUSY, INPUT);
	pinMode(Pin_RESET, OUTPUT);
	pinMode(Pin_PANEL_ON, OUTPUT);
	pinMode(Pin_DISCHARGE, OUTPUT);
	pinMode(Pin_BORDER, OUTPUT);
	pinMode(Pin_EPD_CS, OUTPUT);
	pinMode(Pin_FLASH_CS, OUTPUT);

	//Set control pins to initial values
	digitalWrite(Pin_RED_LED, LOW);
	digitalWrite(Pin_PWM, LOW);
	digitalWrite(Pin_RESET, LOW);
	digitalWrite(Pin_PANEL_ON, LOW);
	digitalWrite(Pin_DISCHARGE, LOW);
	digitalWrite(Pin_BORDER, LOW);
	digitalWrite(Pin_EPD_CS, LOW);
	digitalWrite(Pin_FLASH_CS, HIGH);

	Serial.begin(9600); //Start serial port

	FLASH.begin(Pin_FLASH_CS); //Start flash chip
	if (FLASH.available()) {
		Serial.println("FLASH chip detected OK");
	} else {
		//Maunfacturer diagnostics
		uint8_t maufacturer;
		uint16_t device;
		FLASH.info(&maufacturer, &device);
		Serial.print("unsupported FLASH chip: MFG: 0x");
		Serial.print(maufacturer, HEX);
		Serial.print("  device: 0x");
		Serial.print(device, HEX);
		Serial.println();
	}

	// configure temperature sensor
	S5813A.begin(Pin_TEMPERATURE);
}

//State machine variables and line holder array
static int state = 0;
int n = 0;
uint8_t lne[16];

//Bouncing circle init coordinates
int cx = 20;
int cy = 20;
int r = 8;
long x;
long s;

//Movement variables
int dx = 1;
int dy = 1;

//Incrementors
long j;
int i;
int k;

// main loop
void loop() {
	EPD.begin(); // power up the EPD panel
        EPD.clear(); //Make sure it's clear
        while(1){

      	  //int temperature = S5813A.read(); //Optional temperature compensation
	  //EPD.setFactor(temperature); // adjust for current temperature
          
          //Move the circle position
          cx += dx;
          cy += dy;

	  // check if still on screen, toggle directions in reverse if hitting an edge
          if ((cx < (r+abs(dx)))||(cx > 128-(r+abs(dx)))){
            dx = -1*dx;}
          if ((cy < (r+abs(dy)))||(cy > 96-(r+abs(dy)))){
            dy = -1*dy;}

	  //looping over x,y coords of the circle edge
          for (j = cy-r-abs(dy)-2;j<2+cy+r+abs(dy);j++){
            for(i = (cx-r-abs(dx))/8 - 1;i < 1+(cx+r+abs(dx))/8;i++){
              lne[i] = 0;
              for (k = 0;k<8;k++){ //draw points in location in the circle
                //y = j; x = 8*i+k
                x = 8*i+k;
                s = pow((j-cy),2) + pow((x-cx),2);
                if (s < 100){
                  lne[i] = lne[i]+(1<<(k)); }}}

            // Different options for line type to draw in screen
            //EPD.line(j,lne,0,false,EPD_compensate);
            //EPD.line(j,lne,0,false,EPD_white);
            //EPD.line(j,lne,0,false,EPD_inverse);
            EPD.line(j,lne,0,false,EPD_normal); //Standard draw
          }
        }
        EPD.end();   // power down the EPD panel
}
