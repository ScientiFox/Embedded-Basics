/*
Utility function to change the address of an I2C SRF02 ultrasonic sensor
*/

#include <i2c_t3.h>

void setup() {
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(115200);          // start serial communication at 9600bps

}

void loop(){

  changeAddress(0xE0>>1,0xF6);//Send change address through function
  
  while(1){} //Do nothing!
  
}

//Function to change address
void changeAddress(byte oldAddress, byte newAddress){

  //Send it 0xA0, 0xAA, 0xA5 to prep the device to receive new address
  Wire.beginTransmission(oldAddress);
  Wire.write(byte(0x00));
  Wire.write(byte(0xA0));
  Wire.endTransmission();
  Wire.beginTransmission(oldAddress);
  Wire.write(byte(0x00));
  Wire.write(byte(0xAA));
  Wire.endTransmission();
  Wire.beginTransmission(oldAddress);
  Wire.write(byte(0x00));
  Wire.write(byte(0xA5));
  Wire.endTransmission();

  //Send the new address- device will load it from the buffer into the address memory
  Wire.beginTransmission(oldAddress);
  Wire.write(byte(0x00));
  Wire.write(newAddress);
  Wire.endTransmission();
}
