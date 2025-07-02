/*
Utility example to get the MAC address of a module, for use in node/hub role setting
and direct link establishment.
*/

#include <SoftwareSerial.h>
#include <BKlib.h>

//Initialize the soft serial port and connected BK device
SoftwareSerial BKport(2,3);
BKlib BK(&BKport);

void setup(){
  //prep the native to-pc port and BK soft port
  Serial.begin(9600);
  BKport.begin(38400);

  delay(200); //Give a sec to get fired up
  BK.get_addr(); //Grab the address and print it
}

void loop(){
}
