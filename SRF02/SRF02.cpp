/*
A library to manage SRF02 I2C ultrasonic distance sensors

Supports 3 channels, via Wire, Wire1, and Wire2, making it work on the Teensy
*/

#include "Arduino.h"
#include "SRF02.h"
#include <i2c_t3.h>

//Initialize a sensor with its address and channel on I2C
SRF02::SRF02(int address, int channel){
	addr = address;
	wire_channel = channel;
	started = false; //Flag to note the sensor is idle	
	reading = -1; //No reading yet

};

//Send a measurement start command
bool SRF02::start_measure(){
	if ((started)){return false;} //Skip if already running

	started = true; //Mark flag that it's running
	if (wire_channel == 0){
		Wire.beginTransmission(addr>>1); //Send the address of this sensor to communicate with it
		Wire.write(byte(0x00)); //Send start command
		Wire.write(byte(0x50)); 
		Wire.endTransmission(); //close the communication frame
	}
	if (wire_channel == 1){
		Wire1.beginTransmission(addr>>1); //Same as above but on Wire1
		Wire1.write(byte(0x00)); 
		Wire1.write(byte(0x50)); 
		Wire1.endTransmission();
	}
	if (wire_channel == 2){
		Wire2.beginTransmission(addr>>1); //Same as above but on Wire2
		Wire2.write(byte(0x00)); 
		Wire2.write(byte(0x50)); 
		Wire2.endTransmission();
	}
	timer = millis(); //Set the response timer
	return true; //Note the op finished correctly
}

//Get the read distance from the sensor
bool SRF02::read_distance(){
	
	if ((millis()-timer < 70)|(!started)){
		return false; //Return false if not enough time has passed for a reading, or the start wasn't called
	}

	if (wire_channel == 0){ //For channel 0

		Wire.beginTransmission(addr>>1); //Send the request reading transmission
		Wire.write(byte(0x02));
		Wire.endTransmission();

		Wire.requestFrom(addr>>1, 2); //Get response, 2 bytes

		if (2 <= Wire.available()){ // if two byets available
			reading = Wire.read(); //Read in
			reading = reading << 8; //shift first byte
			reading |= Wire.read(); //Or with byte 2
		}	
	}
	if (wire_channel == 1){ //Same but for Wire1 channel
	
		Wire1.beginTransmission(addr>>1);
		Wire1.write(byte(0x02));
		Wire1.endTransmission();

		Wire1.requestFrom(addr>>1, 2);

		if (2 <= Wire1.available()){
			reading = Wire1.read();
			reading = reading << 8;
			reading |= Wire1.read();
		}
	}
	if (wire_channel == 2){ //Same for Wire2 channel
	
		Wire2.beginTransmission(addr>>1);
		Wire2.write(byte(0x02));
		Wire2.endTransmission();

		Wire2.requestFrom(addr>>1, 2);

		if (2 <= Wire2.available()){
			reading = Wire2.read();
			reading = reading << 8;
			reading |= Wire2.read();
		}
	}
	started = false; //CLear start flag once a reading is pulled
	return true; //Note execution is successful
}



