/*
 *This sketch lets you use one makerboard as a controller for the
 *ST7565 graphic screen which recieves serial commands from another
 *makerboard to tell it what to put on the screen.
 *
 *This sketch pairs with the Serial Screen Controller library, 
 *which goes on the makerboard that is telling the screen 
 *controller what to do. That library contains all the serial
 * commands, and formatting, necessary to talk to this sketch.
 */

#include "ST7565.h"

ST7565 glcd(9, 8, 7, 6, 5); //Create the GLCD object to talk to

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

/*
 * A few utility variables- a character for reading the serial
 * port, a string for parsing, and a couple arrays for data 
 * storage
 */

char s; //Character for string parsing

/*
 * data for the draw- item type, width and height, corner and
 * collision avoidance direction
 */
int d[] = {0,0,0,0,0,0}; //graphic parameters
int da[] = {0,0,0,0,0,0}; //display data (for plots)
String out; //string holder- for drawing words

void setup(){
  /*
   * This side uses the pin 0,1 primary serial port, the other 
   * side uses a software serial port so you can still use 
   * println() to display information on your computer
   */
  Serial.begin(9600);

  /*
   * Initialize the screen contrast- values of 0x01 or 0x00 work
   * best with our screen
   */
  glcd.begin(0x00);

  glcd.clear(); //Clear the screen

  glcd.display(); //And display the blank screen
}

void loop(){
  /*
   * Central loop checks serial port for data and searches 
   * incoming stream for parsing markers
   */
  if (Serial.available()>0){
    s = Serial.read(); //Get data from port
    
    // & is the character indicating the start of a data packet
    if (s == '&'){
    
      //The first 6 integers are the graphical information
      for (int i = 0;i<6;i++){
        d[i] = Serial.parseInt();}
    
      /*
       * The rest is the additional data, like the characters
       * in a string, or values in a graph
       */
      out = "";
      if (d[0] == 5){ //indicates a string, so use String out
        s = Serial.read();
        while (s != ','){s = Serial.read();}
        while (s != '*'){ //'*' is the packet termination character
          s = Serial.read();
          while (s == -1){s = Serial.read();}
          if (s != '*'){
            out = out + s;}
        }
      }
      else{
        //if not reading in a string, it's integer data for da[]
        for (int i=0;i<d[1];i++){
            da[i] = Serial.parseInt();}
      }
      
      switch (d[0]){ //Switch to draw the specific object type
        case (0):
          //Clear the screen
          glcd.clear();
          break;
        case (1):
          //Rectangle
          glcd.write_rect(d[2],d[3],d[4],d[5]);
          break;
        case (2):
          //Circle
          glcd.write_circle(d[2],d[4],d[5]);
          break;
        case (3):
          //Filled rectangle
          glcd.write_rect_fill(d[2],d[3],d[4],d[5]);
          break;
        case (4):
          //filled circle
          glcd.write_circle_fill(d[2],d[4],d[5]);
          break;
        case (5):
          //String
          glcd.write_string(out,d[4],d[5]);
          break;
        case (6):
          //Graph
          glcd.write_graph(da,6,d[3],d[2],d[4],d[5]);
          break;
        case (7):
          //Thermometer graphic
          glcd.write_thermometer(d[2],d[4],d[5]);
          break;
        case (8):
          //Rising sun graphic
          glcd.write_sun(d[4],d[5]);
          break;
        case (9):
          //Full sun graphic
          glcd.write_full_sun(d[4],d[5]);
          break;
        case (10):
          //Clouds graphic
          glcd.write_cloud(d[4],d[5]);
          break;
        case (11):
          //Clock graphic
          glcd.write_clock(d[2],d[3],10,d[4],d[5]);
          break;
        case (12):
          //'invisible' rectangle- for positioning objects
          glcd.write_rect_invis(d[2],d[3],d[4],d[5]);
          break;
        case (999):
          //Draw everything plotted so far
          glcd.display();
          break;
      }
    }
  }
}



