/*
Basic BKlib implementation example, using a known port
*/

#include <SoftwareSerial.h>
#include <BKlib.h>

//Initialize the soft serial port and connected BK device
SoftwareSerial BKport(2,3);
BKlib BK(&BKport);

//Known target MAC address from BKlib_get_addr
String Node1_MAC = "2014,4,143378";

void setup(){
  //prep the native to-pc port and BK soft port
  Serial.begin(9600);
  BKport.begin(38400);
  delay(200); //Wait a bit for warmup
  BK.initialize(Node1_MAC); //Initialize the connection
}

void loop(){
  //Send a basic string to receiver
  Serial.println("Sending...");
  BKport.println("Receiving!");
  delay(1000); //Do it every 1 second
}
