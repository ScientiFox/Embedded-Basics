//Include library
#include <Ping.h>

//Make the object
Ping ping = Ping(13);

void setup(){
  Serial.begin(115200); //Serial init for reporting
}

void loop(){

  ping.fire(); //Send a ping

  //Display results
  Serial.print("Microseconds: ");
  Serial.print(ping.microseconds());
  Serial.print(" | Inches ");
  Serial.print(ping.inches());
  Serial.print(" | Centimeters: ");
  Serial.print(ping.centimeters());
  Serial.println();
}
