/*
A wrapper library for Arduino modules to interface with serial-over bluetooth modules.

Takes advantage of the basic hub/node architecture provided by BK modules using
the built in AT command set available for bluetooth modules as modems!

*/

//Basic imports- uses software serial to leave HW serial available for to-pc comms
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <BKlib.h>

//Basic class- just needs to keep a port, methods handle all logic
BKlib::BKlib(SoftwareSerial* port){
	_port = port;
}

//Command to initialize a connection to another module, with a known MAC address
void BKlib::initialize(String MAC){

	//First attention grab
	String command = "AT\r\n";
	call_and_response(command);

	//Start up command
	command = "AT+INIT\r\n";
	call_and_response(command);

	//Inquire command
	command = "AT+INQ\r\n";
	call_and_response(command);

	//Pairing command
	command = "AT+FSAD="+MAC+"\r\n";
	call_and_response(command);

	//Link establish command
	command = "AT+LINK="+MAC+"\r\n";
	call_and_response(command);
}

//Function to add a known link without an inquire
void BKlib::add_link(String MAC){

	//Attention command
	String command = "AT\r\n";
	call_and_response(command);
	call_and_response(command); //send twice to ensure module is listening

	//Init command
	command = "AT+INIT\r\n";
	call_and_response(command);

	//command = "AT+INQ\r\n";
	command = "AT+FSAD="+MAC+"\r\n"; //Directly pair w/o inquire
	call_and_response(command);

	//Open loop link to- won't work if comms times out, fails soft
	command = "AT+LINK="+MAC+"\r\n";
	call_and_response(command);	
}

//Function to send a message and print the reply
void BKlib::call_and_response(String command){
	(*_port).print(command); //print the command to the stream on the port (faster than write)
	Serial.print(command); //Echo command to native hw serial port
 	while(!(*_port).available()){} //Wait until data on the port
  	while((*_port).available()){ //while there's data
	    	Serial.print(char((*_port).read())); //read in to hw serial port
    		delay(10); //wait a bit ofr more data
    	}
}

//Wrapper for AT command to ger the address of the module
void BKlib::get_addr(){
	call_and_response("AT+ADDR?\r\n");
}

//Wrapper for AT commands to make this module a hub
void BKlib::set_hub(){
	call_and_response("AT+ROLE=1\r\n"); //send command
	call_and_response("AT+ROLE?\r\n"); //verify
}

//Wrapper to make this module a node
void BKlib::set_node(){
	call_and_response("AT+ROLE=0\r\n"); //send command
	call_and_response("AT+ROLE?\r\n"); //verify
}
