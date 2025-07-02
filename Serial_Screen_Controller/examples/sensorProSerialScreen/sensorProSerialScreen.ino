/*
 * This sketch runs the sensor Pro project- the all-purpose
 * weather station- on a graphic LCD screen via the serial screen
 * controller library. This allows the Parent, to which all the
 * sensors are connected, have more memory and processing
 * power available for handling data, leaving the heavy lifting
 * on graphics to the child.
 *
 * In order to make this work, the parent
 * communicates over serial to the child, sending data in a packet
 * format specified in the serial screen controller library.
 */

//Several includes- the LCD, I2C, RTC, Pressure, and DHT22 libraries
#include "SoftwareSerial.h"
#include "Serial_Screen_Controller.h"
#include "Wire.h"
#include "RTClib.h"
#include "SFE_BMP180.h"
#include "DHT.h"

//The DHT22 library requires these #defines:
#define DHTPIN 12
#define DHTTYPE DHT22

//These are objects which handle the special peripherals
RTC_DS1307 rtc; //The RTC
DHT dht(DHTPIN, DHTTYPE); //The DHT22
SFE_BMP180 pressure; //The pressure sensor
Serial_Screen_Controller screen; //The LCD screen

//We define a couple utility variables

//For the temp, humidity, and pressure measurements
double T,H,T2,P; 

int light; //The value from the photocell

//The graph data from the sound sensor
int sound_data[] = {500,500,500,500,500,500};

//The sensor reading from the microphone
int sound_avg = analogRead(A1);

//a timer for the measurements and blinking things
long timer = millis();

//a state variable to blink the  ':' on the clock
byte clock_blink = 0;

//required variable for the pressure sensor
char status;

//A string to hold the data packet for transmission
String dataString;

void setup() {
  /* 
   * start the serial port (not necessary, but our I2C sensors)
   * use it to let you know if there's a problem
   */
  Serial.begin(9600);

  screen.clr(); //Initialize the Screen

  //set A0 to input pullup to bias the photocell
  pinMode(A0,INPUT_PULLUP); 
  
  //set pin 12 to input pullup because the DHT22 wants a pullup
  pinMode(12,INPUT_PULLUP);

  //start the I2C library for the RTC and the presure sensor
  Wire.begin();

  rtc.begin(); //Initialize the RTC
  if (! rtc.isrunning()) {
    /*
     * The following line sets the RTC to the 
     * date & time this sketch was compiled
     */
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  if (pressure.begin()){ //Initialize the pressure sensor
    Serial.println("BMP180 init success");}
  else{
    Serial.println("BMP180 init fail");}
}

void loop() {
  
  /*
   * This first big chunk reads in all the sensor values,
   * to get ready for the plotting part later
  */
  
  /*
   * read temp and pressure from the BMP180
   * this is a bunch of code from the example
   * don't worry about exactly how it works, just 
   * keep it as one block
   */
  status = pressure.startTemperature();
  if (status != 0){
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0){
      status = pressure.startPressure(3);
      if (status != 0){
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0){
          //change the read temperature to Farenheit
          T = (9*(T/5))+32;
          }
        }
      }
    }
  
  
  //update the sound data graph 2x per second
  if (timer - millis()>500){
    for (int i=0;i<5;i++){
      sound_data[i+1] = sound_data[i];} //shift out old data
    //shift in new data- /10 for scaling
    sound_data[0] = sound_avg/10; 
  }

  //read in the new sound value and average it with the old:
  sound_avg = (9*(float)sound_avg + analogRead(A1))/10;

  //read temp & humidity from the DHT22
  T2 = dht.readTemperature(true);
  if (int(T2) == -999){} //check for read fault
  else{T2 = ((9*T2)/5)+32;} //convert to farenheit
  H = dht.readHumidity();

  //get the current date and time from the RTC
  DateTime now = rtc.now();

  //read the value on the photocell
  light = analogRead(A0);

  /*
   * The next chunk formats and displays all the data we just read
   * on the screen, and draws all the special icons and things (like
   * the thermometer and clock)
   */

  screen.clr(); //wipe the screen clear

/*
 * Draw the thermometer and the measured temperature
 * we use (T+T2)/2 because it's the average of the DHT22
 * reading and the Pressure sensor reading
 */
  screen.draw_thermometer(int((T+T2)/2), BOTTOM_LEFT, VERTICAL);
  screen.draw_string(String(int((T+T2)/2))+" F",BOTTOM_LEFT,HORIZONTAL);

  //Draw the date and time read out in the bottom right corner
  draw_time(now.hour(),now.minute(),BOTTOM_RIGHT,HORIZONTAL);
  screen.draw_rect_invis(8,5,BOTTOM_RIGHT,HORIZONTAL);
  screen.draw_string(String(now.month())+"/"+String(now.day()),BOTTOM_RIGHT,HORIZONTAL);

  /* Draw the graph of the sound data, and next to it
   * the sound reading (VU just means arbitrary volume units-
   * we don't have a conversion)
   */
  screen.draw_graph(sound_data, 6, 17, 100, TOP_LEFT, VERTICAL);
  screen.draw_string(String(int(sound_avg))+" VU",TOP_LEFT,HORIZONTAL);

  //Draw the humidity reading next to the thermometer
  screen.draw_string(String(int(H))+" %RH",BOTTOM_LEFT,HORIZONTAL);

  //Draw the clock over top of the date and time 
  screen.draw_rect_invis(8,20,2,1); //for spacing, again
  screen.draw_clock(now.hour(),now.minute(), BOTTOM_LEFT, HORIZONTAL);

  /*
   * Draw the Pressure reading above the humidity and temperature
   * For spacing, we use invisible rectangles.
   * If you have trouble using these, try making
   * them visible at first, then invisible once you get
   * everything laid out correctly.
   */
  screen.draw_rect_invis(20,15,BOTTOM_LEFT,HORIZONTAL);
  screen.draw_rect_invis(6,40,BOTTOM_LEFT,HORIZONTAL); 
  screen.draw_rect_invis(6,30,BOTTOM_LEFT,HORIZONTAL);
  screen.draw_rect_invis(6,26,BOTTOM_RIGHT,HORIZONTAL);
  screen.draw_string(String(int(P))+" bar",BOTTOM_LEFT,HORIZONTAL);

  /*
   * Draw the sun or cloud graphic in the upper right corner
   * depending on the light level reading
   */
  if (light > 200){
    screen.draw_cloud(TOP_RIGHT,VERTICAL);}
  else if (light > 80){
    screen.draw_sun(TOP_RIGHT,VERTICAL);}
  else{
    screen.draw_sun_full(TOP_RIGHT,VERTICAL);}

  //put up the display, now that all the drawing's done
  screen.draw();
  
  delay(100); //Stability delay
}

void draw_time(byte hr, byte mn, byte crn, byte dir){
  /*
   * This is a utility function to draw the time neatly 
   * like not writing 10:02 as 10:2 and blinking the ':' 
   */

  if (millis()-timer>500){
    timer = millis(); //Change the state 2x per second
    clock_blink = 1 - clock_blink;}

  if (clock_blink){
    //in one state draw the time without the ':'
    if (mn<10){
      //and remember the extra '0' if less than 10
      screen.draw_string(String(hr)+" 0"+String(mn),  crn, dir);}
    else{
      screen.draw_string(String(hr)+" "+String(mn), crn, dir);}
  }
  else{
    if (mn<10){ //in the other state, draw the ':'
      //and don't forget the '0' if you need it
      screen.draw_string(String(hr)+":0"+String(mn), crn, dir);}
    else{
      screen.draw_string(String(hr)+":"+String(mn), crn, dir);}
  }
}


