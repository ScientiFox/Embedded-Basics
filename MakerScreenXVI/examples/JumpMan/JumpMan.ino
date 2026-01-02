/*
 * This sketch is a game for the LCD carrier where you control
 *  your character with a button and try not to run into
 *  obstacles. It's a good starting point for how to make a game
 *  using the LCD.
 */

//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Make an LCD display object, these numbers are the pins that the
//  Maker Screen uses
MakerScreenXVI lcd;

/*
 * We make a bunch of custom graphics for the LCD screen, to 
 *  animate our jumping guy, the walls, and hearts
 */
byte run1[8] = {
  0b00000,
  0b01100,
  0b01100,
  0b01100,
  0b01110,
  0b01100,
  0b01010,
  0b01010
};

byte run2[8] = {
  0b00000,
  0b01100,
  0b01100,
  0b01111,
  0b01100,
  0b01100,
  0b01010,
  0b10001
};

byte wall[8] = {
  0b11111,
  0b10101,
  0b11111,
  0b10001,
  0b11111,
  0b10101,
  0b11111,
  0b10001
};

byte up[8] = {
  0b01010,
  0b01010,
  0b01110,
  0b00000,
  0b01110,
  0b01010,
  0b01110,
  0b01000
};

byte crash[8] = {
0b00000,
  0b00011,
  0b01011,
  0b00111,
  0b00011,
  0b00011,
  0b00001,
  0b00001
};

byte jump[8] = {
  0b00110,
  0b10110,
  0b01100,
  0b01111,
  0b01110,
  0b01011,
  0b10000,
  0b00000
};

byte heart1[8] = {
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};

//A bunch of game variables tracking different things
int lives = 3; //The number of lives the player has
int dudePosition = 0; //An index tracking the jump animation cycle
int wallPosition = 15; //The location of the wall
int dudeRunState = 0; //An index tracking the run animation cycle
int crashFlag = 0; //A flag to say when the guy has crashed

long timer = millis(); //General purpose timer for everything
int ticks = 0; //The number of animation frames counted
int gameOver = 0; //A flag for when the user runs out of lives
int button = 1; //Variable holding the button state
int duration = 500; //Durarion in ms of 1 animation frame
int heartOn = 0; //Whether there is a heart over the wall this round
int resetState = 0; //Flag for when to reset the game
int lastPoints = 0; //The number of points scored by the last player

void setup() {

  //Set up the LCD's number of columns and rows:
  lcd.begin();
  
  //Make all the new characters in the LCD
  lcd.createChar(0, run1);
  lcd.createChar(1, run2);
  lcd.createChar(2, wall);
  lcd.createChar(3, up);
  lcd.createChar(4, crash);
  lcd.createChar(5, jump);
  lcd.createChar(6, heart1);

  //Set up the button pin
  pinMode(6,INPUT_PULLUP);

  //Display the start-up message
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write("Press Button");
  lcd.setCursor(4,1);
  lcd.write("to Start");

  lcd.backlightOn();
  
  //Wait for the first button press to begin
  while (digitalRead(6)){}
  delay(6);
  while (1-digitalRead(6)){}
  delay(10);

}

void loop() {

  //Resetting the game
  if (resetState == 1){
    //Display start message again
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.write("Press Button");
    lcd.setCursor(4,1);
    lcd.write("to Start");
    lcd.setCursor(0,1);
    lcd.print(lastPoints); //Display the last player's points

    //Wait for the button press
    while (digitalRead(6)){}
    delay(10);
    while (1-digitalRead(6)){}
    delay(10);

    //Clear the reset flag
    resetState = 0;
  }

  //Read the button, ignore it if in the middle of jumping
  if (digitalRead(6) == 0){
    button = (dudePosition != 0);}

  //Draw the wall
  lcd.setCursor(wallPosition,1);
  lcd.write(byte(2));
  /*
   * Draw the heart if there is one currently and it's not
   *  drawing over the lives displat in the top right
   */
  if (heartOn&(wallPosition!=14)&(wallPosition!=13)){
    lcd.setCursor(wallPosition,0);
    lcd.write(byte(6));}
    
  //Draw the lives display in the top right
  if (lives > 9){lives = 9;} //Limit of 9 lives
  lcd.setCursor(13,0);
  lcd.write(byte(5));
  lcd.setCursor(14,0);
  lcd.print(lives);

  //Check the position of the guy
  lcd.setCursor(0,1);

  //A suite of checks to see if the guy should crash  
  if (((dudePosition == 0)|(dudePosition == 3)|(dudePosition == 4))&((wallPosition == 1)|(wallPosition == 0))&(crashFlag == 0)){
    crashFlag = 1;
  }
  else if ((dudePosition == 2)&(wallPosition == 1)&(crashFlag == 0)){
    crashFlag = 1;
  }
  else if (crashFlag>0){
    lcd.write(byte(4)); //Draw the 'crashed' dude if he has crashed
  }
  //Draw him jumping over the wall if he's positioned correctly
  else if ((dudePosition > 0)&(dudePosition<2)){
    lcd.setCursor(1,0);
    lcd.write(byte(5));
    //Add a life if positioned correctly and there is a heart
    if ((heartOn)&(wallPosition == 1)){
      lives++;
      heartOn = 0; //Clear the heart once you get it
      tone(A5,1200); //Happy beep for 1up 
    }
  }
  //Draw the  dude after the wall if jump is done
  else if (dudePosition == 2){
    lcd.setCursor(1,1);
    lcd.write(byte(1));
    dudeRunState = 1; //Pick up run from second frame of animation
    noTone(A5); //CLear and sounds being made
  }

  //If no jumping or crashing, draw the run animation cycle
  else if (dudeRunState){
    lcd.write(byte(0));}
  else{
    lcd.write(byte(1));}

  //What to do when the player runs out of life
  if (gameOver){

    //Print "Game Over" and the number of points
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Game");
    lcd.setCursor(9,0);
    lastPoints = (ticks/15) - 3; //Points computation
    lcd.print(lastPoints);
    lcd.setCursor(2,1);
    lcd.print("Over");
    lcd.setCursor(9,1);
    lcd.print("Points");
    delay(3000); //Wait 3 seconds to display
    gameOver = 0; //Reset game over flag
    resetState = 1; //Set reset flag to start fresh
    lives = 3; //Set lives at original count
    duration = 500; //Set game speed at starting point
    wallPosition = 15; //Put the wall back at the right
    ticks = 0; //Clear the frame counter
  }

  //Animation loop timing- duration controls the speed
  if (millis()-timer>duration){

    lcd.clear(); //Wipe the screen
   
    //Check if the button is pressed
    if (button == 0){
      dudePosition = 1; //Start the jump animation if so
      button = 1;
      if (crashFlag == 0){ //Set a jump noise if not crashing
        tone(A5,400);}
    }
    else if ((dudePosition < 3)&(dudePosition != 0)){
      dudePosition++; //Increment the animation loop during the jump
    }
    else if (dudePosition == 3){
      //Reset the incrementor if the animation loop is done
      dudePosition = 0;
    }

    //Checking the crash incrementor to play the crash noise
    if (crashFlag>0){
      crashFlag++; //Increment the noise
      if (crashFlag == 2){
        tone(A5,200); //Sad noise for crashing
      }
      if (crashFlag == 3){
        noTone(A5); //Clear the noise before it gets annoying
      }
      if (crashFlag == 4){
        //Reset the flag when duration of crash is over
        crashFlag = 0; 
        lives--; //And lose a life
        wallPosition = 15; //Reset the wall
        if (lives == 0){gameOver = 1;} //And check if it's game over
      }
    }
    //Reset the wall when it's at the end of the screen
    else if (wallPosition == 0){
      ticks = ticks + (1200 - duration)/16; //Increment frame counter

      //Increase the duration
      duration = 500 - (ticks/15 - 3)*10;
      duration = duration*(duration >= 20) + 20*(duration < 20);

      wallPosition = 15; //Reset the wall
    }
    //If no resets or flags, cycle run animation and move the wall
    else{  
      dudeRunState = 1-dudeRunState;
      wallPosition--;
    }

    //When the wall is reset
    if (wallPosition == 15){
      noTone(A5); //Clear tones
      heartOn = random(0,10)<3;} //And make a heart 3/10 times
      
    timer = millis(); //Set the timer for next loop
    ticks++; //Increment the frame counter
  }
  
}
