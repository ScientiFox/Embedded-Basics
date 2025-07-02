/*
Example for setting node/hub role on an attached BK module,
plus demonstrating swapping them and getting MAC address
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

  //Note that you're setting as hub now  
  Serial.println("Set as hub...");
  delay(200); //wait a bit
  BK.set_hub(); //set as hub

  delay(2000); //Wait 2 seconds

  Serial.println("Set as node..."); //Note it's node time
  delay(200); //Wait a bit
  BK.set_node(); //Make it a node

  delay(2000); //wait 2 secs

  Serial.println("Get MAC address..."); //Note we're grabbing the mAC
  delay(200); //wait a bit
  BK.get_addr(); //Get the MAC address

  delay(2000); //wait 2 secs

  Serial.println("Done"); //And finished
}

void loop(){
}
