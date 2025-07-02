
#include <inttypes.h>
#include <ctype.h>

// required libraries
#include <SPI.h>
#include <FLASH.h>
#include <EPD.h>
#include <S5813A.h>

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


#define EPD_SIZE EPD_1_44


// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS);


// I/O setup
void setup() {
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

	digitalWrite(Pin_RED_LED, LOW);
	digitalWrite(Pin_PWM, LOW);
	digitalWrite(Pin_RESET, LOW);
	digitalWrite(Pin_PANEL_ON, LOW);
	digitalWrite(Pin_DISCHARGE, LOW);
	digitalWrite(Pin_BORDER, LOW);
	digitalWrite(Pin_EPD_CS, LOW);
	digitalWrite(Pin_FLASH_CS, HIGH);

	Serial.begin(9600);

	FLASH.begin(Pin_FLASH_CS);
	if (FLASH.available()) {
		Serial.println("FLASH chip detected OK");
	} else {
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

static int state = 0;
int n = 0;
uint8_t lne[16];

int cx = 20;
int cy = 20;
int r = 15;
long x;
long s;

long j;
int i;
int k;

// main loop
void loop() {

	EPD.begin(); // power up the EPD panel
        EPD.clear();
      while(1){
	int temperature = S5813A.read();
	EPD.setFactor(temperature); // adjust for current temperature
        //EPD.clear();

        cx += 5;
        cy += 5;

        for (j = cy-r-5;j<cy+r+5;j++){
          for(i = (cx-r-5)/8;i < (cx+r+5)/8;i++){
            lne[i] = 0;
            for (k = 0;k<8;k++){
              //y = j; x = 8*i+k
              x = 8*i+k;
              s = pow((j-cy),2) + pow((x-cx),2);
              if (s < 100){
                lne[i] = lne[i]+(1<<(k)); }}}
          EPD.line(j,lne,0,false,EPD_compensate);
          //EPD.line(j,lne,0,false,EPD_white);
          //EPD.line(j,lne,0,false,EPD_inverse);
          EPD.line(j,lne,0,false,EPD_normal);
        }

        //n = 0;
        //while (n < 96){
        //  EPD.line(n,lne[n],0,false,EPD_compensate);
        //  EPD.line(n,lne[n],0,false,EPD_white);
        //  EPD.line(n,lne[n],0,false,EPD_inverse);
        //  EPD.line(n,lne[n],0,false,EPD_normal);
        //  n++;}
}
	EPD.end();   // power down the EPD panel

}
