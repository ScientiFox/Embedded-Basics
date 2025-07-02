/*
 * This is a very large sketch which brings together the graphic
 * LCD screen, Pressure sensor, DHT22 temp/humidity sensor,
 * Real time clock, Microphone, and photocell to make a
 * continuous display weather station.
 * 
 * It draws on the screen:
 *   A sun or cloud image, based on the light level, in the upper
 *     right corner
 *   A graph of the noise level and the measured level in the upper
 *     left corner
 *   A thermometer indicating the temperature, and the current 
 *    measured temperature, atmospheric pressure, and humidity
 *    in the bottom left
 *   The date, time, and a clock inthe bottom right hand corner
 */

/*
 * Several includes- the LCD, I2C, RTC, Pressure, and DHT22 
 * libraries
 */
#include "ST7565.h"
#include "Wire.h"
#include "RTClib.h"
#include "SFE_BMP180.h"
#include "DHT.h"

//The LCD and DHT22 libraries require these #defines:
#define DHTPIN 12
#define DHTTYPE DHT22
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

//These are objects which handle the special peripherals
RTC_DS1307 rtc; //The RTC
ST7565 glcd(9, 8, 7, 6, 5); //The graphic LCD
DHT dht(DHTPIN, DHTTYPE); //The DHT22
SFE_BMP180 pressure; //The pressure sensor

//We define a couple utility variables

//For the temp, humidity, and pressure measurements
double T,H,T2,P;

int light; //The value from the photocell

//The graph data from the sound sensor
int sound_data[] = {500,500,500,500,500};

//The sensor reading from the microphone
int sound_avg = analogRead(A1);

//a timer for the measurements and blinking things
long timer = millis();

//a state variable to blink the  ':' on the clock
byte clock_blink = 0; 

//required variable for the pressure sensor
char status;

//A string to hold data for display
String dataString;

void setup() {
  /*
   * start the serial port (not necessary, but our I2C sensors)
   * use it to let you know if there's a problem
   */
  Serial.begin(9600);

  //Initialize the Screen
  glcd.begin(0x01); //Contrast set- 01 is best for this screen
  glcd.clear();  //make sure the display is blank
  glcd.display(); //and draw nothingnss

  //set A0 to input pullup to bias the photocell
  pinMode(A0,INPUT_PULLUP); 
  
  //set pin 12 to input pullup because the DHT22 wants a pullup
  pinMode(12,INPUT_PULLUP);

  //start the I2C library for the RTC and the presure sensor
  Wire.begin();

  //Initialize the RTC
  rtc.begin();
  if (! rtc.isrunning()) {
    /*
     * following line sets the RTC to the date &
     * time this sketch was compiled
     */ 
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  //Initialize the pressure sensor
  if (pressure.begin()){
    Serial.println("BMP180 init success");}
  else{
    Serial.println("BMP180 init fail");}
    
  //Initialize the SD card
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");}
  else{
    Serial.println("card initialized.");}
}

void loop() {
  
  /*
   * This first big chunk reads in all the sensor values,
   * to get ready for the plotting part later
   */
  
  //read temp and pressure from the BMP180
  status = pressure.startTemperature();
  if (status != 0){
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0){
      status = pressure.startPressure(3);
      if (status != 0){
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0){}}}}
        
  //change the temperature to Farenheit
  T = (9*(T/5))+32;
  
  //update the sound data graph 2x per second
  if (timer - millis()>500){
    for (int i=0;i<4;i++){
      sound_data[i+1] = sound_data[i];} //shift out old data
    //shift in new data- /10 for scaling
    sound_data[0] = sound_avg/10;} 

  //read in the new sound value and average it with the old:
  sound_avg = (9*(float)sound_avg + analogRead(A1))/10;

  //read temp & humidity from the DHT22
  T2 = dht.readTemperature(true);
  H = dht.readHumidity();

  //get the current date and time from the RTC
  DateTime now = rtc.now();

  light = analogRead(A0); //read the value on the photocell

  /*
   * Construct the a string to hold the data read from the sensor
   * and the date and time
   */
  dataString = String(now.month())+"/"+String(now.day())+String(now.hour())+":"+String(now.minute())+",P "+String(P)+",T"+String((T+T2)/2)+",H "+String(H);
  Serial.println(dataString);
 
  /*
   * The next chunk formats and displays all the data we just read
   * on the screen, and draws all the special icons and things (like
   * the thermometer and clock)
   */  
  
  glcd.clear(); //wipe the screen clear
  
  /*
   * Draw the thermometer and the measured temperature
   * we use (T+T2)/2 because it's the average of the DHT22
   * reading and the Pressure sensor reading
   */
  glcd.write_thermometer(int((T+T2)/2), 2, 0);
  glcd.write_string(String(int((T+T2)/2))+" F",2,1);

  /*
   * Draw the read out date and time in the bottom right corner 
   * draw the time, see function below we use the 
   * 'invisible rectangles' for spacing- they draw blank space
   */
  draw_time(now.hour(),now.minute(),3,1);
  glcd.write_rect_invis(8,5,3,1);
  glcd.write_string(String(now.month())+"/"+String(now.day()),3,1);

  //Draw the humidity reading next to the thermometer
  glcd.write_string(String(int(H))+" %RH",2,1);

  //Draw the clock over top of the date and time 
  glcd.write_rect_invis(8,20,2,1); //for spacing, again
  glcd.write_clock(now.hour(),now.minute(), 10, 2, 1);

 /*
  * Draw the graph of the sound data, and next to it
  * the sound reading (VU just means arbitrary volume units-
  *  we don't have a conversion)
  */
  glcd.write_graph(sound_data, 5, 100, 20, 0, 0);
  glcd.write_string(String(int(sound_avg))+" VU",0,1);

  /*
   * Draw the Pressure reading above the humidity and temperature
   * For spacing, we use invisible rectangles.
   * If you have trouble using these, try making
   * them visible at first, then invisible once you get
   * everything laid out correctly.
   */  
  glcd.write_rect_invis(20,15,2,1); 
  glcd.write_rect_invis(6,40,2,1); 
  glcd.write_rect_invis(6,30,2,1); 
  glcd.write_rect_invis(6,26,3,1); 
  glcd.write_string(String(int(P))+" bar",2,1);

  /*
   * Draw the sun or cloud graphic in the upper right corner
   * depending on the light level reading
   */
  if (light > 200){
    glcd.write_cloud(1,0);}
  else if (light > 80){
    glcd.write_sun(1,0);}
  else{
    glcd.write_full_sun(1,0);}

  //put up the display, now that all the drawing's done
  glcd.display();

  delay(100); //stability delay
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
      glcd.write_string(String(hr)+" 0"+String(mn),  crn, dir);}
    else{
      glcd.write_string(String(hr)+" "+String(mn), crn, dir);}
  }
  else{
    //in the other state, draw the ':'
    if (mn<10){
      //and don't forget the '0' if you need it
      glcd.write_string(String(hr)+":0"+String(mn), crn, dir);}
    else{
      glcd.write_string(String(hr)+":"+String(mn), crn, dir);}
      
      
  }
}
// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
