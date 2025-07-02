/*
Demo of the EPD library managing and executing real time drawing utilities

Shows a bouncing ball within the display area!
*/

//Libraries
#include <Durham_EPD.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <FLASH.h>
#include <EPD.h>
#include <S5813A.h>

//definitions- #def saves SRAM
#define Pin_TEMPERATURE A0
#define Pin_PANEL_ON 2
#define Pin_BORDER 3
#define Pin_DISCHARGE 4
#define Pin_PWM 5
#define Pin_RESET 6
#define Pin_BUSY 7
#define Pin_EPD_CS 8
#define Pin_FLASH_CS 9
#define Pin_SW2 12
#define Pin_RED_LED 13
#define LED_ON  HIGH
#define LED_OFF LOW
#define EPD_SIZE EPD_1_44

//Base EPD class for pervasive displays
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS);

//name the epd
#define D_EPD ep
Durham_EPD D_EPD;

//Incrementors and bmp holder array
int i,j,k,l;
int s[8];

void EPD_draw(){
  //Function to draw to screen

  for (i = 0; i< 8;i++){ //looping over array
    for(j = 0;j<12;j++){
      for (k = 0;k<16;k++){ //and draw object index
        if (D_EPD.ch_map[i][k] != 26){D_EPD.write_letter(k,D_EPD.ch_map[i][k],j);} //If not null, write a character
        else{D_EPD.lne[k] = 0;}}
      for(k=0;k<8;k++){ //looping over bmp holder
        for (l = 7;l>=0;l--){s[l] = (D_EPD.bmp[(12*i+j)/2][k]&(1<<l))>0;} //de-indexing bmp holder
        D_EPD.lne[2*k+1] = D_EPD.lne[2*k+1]|((s[0]<<7)|(s[0]<<6)|(s[1]<<5)|(s[1]<<4)|(s[2]<<3)|(s[2]<<2)|(s[3]<<1)|(s[3]<<0)); //bit-shift 4-part maps into parts
        D_EPD.lne[2*k] = D_EPD.lne[2*k]|((s[4]<<7)|(s[4]<<6)|(s[5]<<5)|(s[5]<<4)|(s[6]<<3)|(s[6]<<2)|(s[7]<<1)|(s[7]<<0));} //same at next index part
      EPD.line(i*12+j,D_EPD.lne,0,false,EPD_normal); //write an epd line
    }}}

void init_EPD(){
  //Initialize the display
  
  //Set the control pin modes
  pinMode(Pin_RED_LED, OUTPUT);
  pinMode(Pin_SW2, INPUT);
  pinMode(Pin_TEMPERATURE, INPUT);
  pinMode(Pin_PWM, OUTPUT);
  pinMode(Pin_BUSY, INPUT);
  pinMode(Pin_RESET, OUTPUT);
  pinMode(Pin_PANEL_ON, OUTPUT);
  pinMode(Pin_DISCHARGE, OUTPUT);
  pinMode(Pin_BORDER, OUTPUT);
  pinMode(Pin_EPD_CS, OUTPUT);
  pinMode(Pin_FLASH_CS, OUTPUT);

  //Start up the control pins to init values
  digitalWrite(Pin_RED_LED, LOW);
  digitalWrite(Pin_PWM, LOW);
  digitalWrite(Pin_RESET, LOW);
  digitalWrite(Pin_PANEL_ON, LOW);
  digitalWrite(Pin_DISCHARGE, LOW);
  digitalWrite(Pin_BORDER, LOW);
  digitalWrite(Pin_EPD_CS, LOW);
  digitalWrite(Pin_FLASH_CS, HIGH);

  Serial.begin(9600); //Init serial port

  FLASH.begin(Pin_FLASH_CS); //activate the flash chip
  if (FLASH.available()) {Serial.println("FLASH chip detected OK");} 
  else{Serial.println("Problem with FLASH chip");}

  //start the S5813A controller
  S5813A.begin(Pin_TEMPERATURE);
  
}

void setup() {
  init_EPD(); //Initialize epd
  D_EPD.clear(); //make sure screen is clear

  //load bytes into bmp for drawing
  for(i=4;i<20;i+=4){
    for(j=2;j<4;j++){
      D_EPD.bmp[i][j] = 0xcc;
      D_EPD.bmp[i+1][j] = 0xcc;
      D_EPD.bmp[i+2][j] = 0x33;
      D_EPD.bmp[i+3][j] = 0x33;
    }}
  for(i=25;i<30;i+=1){
    for(j=3;j<6;j++){
      D_EPD.bmp[i][j] = 0xFF;
    }}
    
  //48x64 scren, define radius and bounds for ball element
  int cy = 40;
  int cx = 30;
  int r = 5;
  for(i=cy-r;i<cy+r;i++){
    for(j=(cx-r)/8;j<1+(cx+r)/8;j++){
      for (k=0;k<8;k++){
        l = 8*j+(k);
        long p = pow(cx-l,2)+pow(cy-i,2);
        if ((p < pow(r,2)+6)&(p > pow(r,2)-12)){
          D_EPD.bmp[i][j] = D_EPD.bmp[i][j]|(1<<(7-k));}
      }}}
      
  //Set up initial partial X and Y position and directions and move ball
  int x = 5;
  int y = 5;
  int dy = 2;
  while (x < 17){
    j = x/8;
    i = x-j*8;
    Serial.print(j); //output coords
    Serial.print(" ");
    Serial.println(i);
    for (l=0;l<dy;l++){
      D_EPD.bmp[y+l][j] = D_EPD.bmp[y+l][j]|(1<<(7-i));} //Shift over bmp rows for movement
    y+=dy;
    x++;}
  
  //Same, but in reverse
  x = 30;
  y = 30;
  dy = 2;
  while (x < 38){
    j = x/8;
    i = x-j*8;
    Serial.print(j);
    Serial.print(" ");
    Serial.println(i);
    for (l=0;l<dy;l++){
      D_EPD.bmp[y-l][j] = D_EPD.bmp[y-l][j]|(1<<(7-i));}
    y-=dy;
    x++;}
  
}
  
void loop() {
  EPD.begin(); // power up the EPD panel
  EPD.clear(); //make sure it's cleared

  //Set up text position and change rate
  int x = 2;
  int y = 2;
  int dx = 1;
  int dy = 1;
  while(1){    //loop forever
    D_EPD.write(x,y,"hello");//write hello to screen at known position
    x+=dx; //move x and y
    y+=dy;
    dy = dy*(1-2*((y==7)+(y==0))); //change dx and dy at boundaries
    dx = dx*(1-2*((x==11)+(x==0)));

    EPD_draw(); //draw mmio to screen
    D_EPD.clear(); //clear screen
    delay(500); //2Hz update loop
  }
  EPD.end();   // power down the EPD panel
}

