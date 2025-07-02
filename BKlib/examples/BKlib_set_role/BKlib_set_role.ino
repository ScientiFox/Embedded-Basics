/*
Short demo for setting the BK module role to hub or node
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

  delay(200); //Brief warmup delay

  //Set as hub or node
  BK.set_hub(); //Uncomment to make this one the hub
  //BK.set_node(); //Uncomment to make this one a node

}

void loop(){
}
