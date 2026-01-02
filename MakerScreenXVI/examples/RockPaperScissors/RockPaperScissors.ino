/*
 * This sketch lets you play Rock Paper Scissors against the Arduino
 *  using an LCD screen and a couple of buttons
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
byte rock1[8] = {
  0b00000,
  0b01100,
  0b10010,
  0b10001,
  0b11001,
  0b01110,
  0b00000,
  0b00000
};

byte rock2[8] = {
  0b11111,
  0b10011,
  0b01101,
  0b01110,
  0b00110,
  0b10001,
  0b11111,
  0b11111
};

byte paper1[8] = {
  0b11111,
  0b10001,
  0b11111,
  0b10001,
  0b11111,
  0b10001,
  0b11111,
  0b00000
};

byte paper2[8] = {
  0b00000,
  0b01110,
  0b00000,
  0b01110,
  0b00000,
  0b01110,
  0b00000,
  0b00000
};

byte scissors1[8] = {
  0b01010,
  0b01010,
  0b01110,
  0b00100,
  0b01110,
  0b10101,
  0b01010,
  0b00000
};

byte scissors2[8] = {
  0b10101,
  0b10101,
  0b10001,
  0b11011,
  0b10001,
  0b01010,
  0b10101,
  0b11111
};

//Variables to hold the states of the two buttons
int buttonPick;
int buttonGo;

//Which move you've selected (Rock,Paper,Scissors) := (0,1,2)
int select = 0;

//The state of the blinking icons
int blinkState = 0;

//Timer to control blinking and scripting
long timer = millis();

//Reset flag and timer
int resetState = 0;
long resetTimer = millis();

void setup() {
  //Set up the LCD's number of columns and rows
  lcd.begin();
  lcd.clear();

  lcd.backlightOn();

  //Button Pins setup
  pinMode(6,INPUT_PULLUP);
  pinMode(A5,INPUT_PULLUP);

  //Make custom characters
  lcd.createChar(0, rock1);
  lcd.createChar(1, rock2);
  lcd.createChar(2, paper1);
  lcd.createChar(3, paper2);
  lcd.createChar(4, scissors1);
  lcd.createChar(5, scissors2);

  //Initialize Random- necessary for it to actually be random
  randomSeed(analogRead(A5));

  //Display the starting message
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write("Press Play");
  lcd.setCursor(4,1);
  lcd.write("to Start!");

  //Wait for the button press to start
  while (digitalRead(A5)){}
  delay(10);
  while (1-digitalRead(A5)){}
  delay(10);
  lcd.clear();

  //Set the reset timer
  resetTimer = millis();

}

void loop() {
  //If reset flag is set
  if (resetState == 1){
    //Wipe the LCD
    lcd.clear();

    //Display the start message again
    lcd.setCursor(2,0);
    lcd.write("Press Play");
    lcd.setCursor(4,1);
    lcd.write("to Start");

    //Wait for button press to start
    while (digitalRead(A5)){}
    delay(10);
    while (1-digitalRead(A5)){}
    delay(10);

    //Clear the reset options
    resetState = 0;
    resetTimer = millis();
    lcd.clear();
  }

  //Wait for 10 seconds without input to reset the game
  if (millis()-resetTimer > 10000){
    resetState = 1;
  }
  
  //Read the button states
  buttonPick = digitalRead(6);
  buttonGo = digitalRead(A5);

  //if the 'choose' button pressed
  if (1-buttonPick){
    resetTimer = millis(); //Clear the reset timer
    lcd.clear(); //Wipe the LCD
    //Increase the chosen move (%3 makes it loop)
    select = (select+1)%3;
    blinkState = 1; //Reset the blink state
    delay(10); //debounce delay
    while (1-digitalRead(6)){ //Wait for release to continue
      delay(10);}
   }

  //If the 'play move' button is pressed, run the game
  if (1-buttonGo){
    lcd.clear(); //Wipe the LCD
    
    /*
     * A quick little timed count down display with beeps for the
     *  game
     */
    lcd.setCursor(0,0);
    lcd.print("One!");
    delay(300);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Two!");
    delay(300);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Three!");
    delay(300);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Go!");
    delay(400);
    
    lcd.clear();
    resetTimer = millis();

    //Pick the computer's move randomly
    int computerMove = random(0,3);

    //Compute who won- 0 is player, 1 is computer, 2 is tie
    int winner = (2 + select - computerMove)%3;

    //Display both player's moves
    lcd.setCursor(0,0);
    //2x the 'select' is the code of the player's choice
    lcd.write(byte(2*select)); 
    lcd.setCursor(15,0);
    lcd.write(byte(2*computerMove));
    lcd.setCursor(1,1);
    //Print a message saying who won, based on 'winner'
    switch (winner){
      case (0):
        lcd.print(" Player wins!");
        break;
      case (1):
        lcd.print("Computer wins!");
        break;
      case (2):
        lcd.print("  It's a Tie!");
        break;
    }
    /*
     * Wait for either a button press or a brief delay to restart
     * the game
     */
    while (digitalRead(A5)&(millis()-timer<3500)){
      delay(10);}
    while ((1-digitalRead(A5))&(millis()-timer<3500)){
      delay(10);}
    
    lcd.clear(); //Clear the screen for the next drawing set
  }

  /*
   * This runs when the game isn't being played. It draws the
   * player's currently selected move and blinks the icon.
   */
  if ((select == 0)&(blinkState == 0)){
    lcd.setCursor(2,0);
    lcd.write(byte(0));
    lcd.setCursor(0,1);
    lcd.print("Rock");
  }
  if ((select == 0)&(blinkState == 1)){
    lcd.setCursor(2,0);
    lcd.write(byte(1));
    lcd.setCursor(0,1);
    lcd.print("Rock");
  }
  if ((select == 1)&(blinkState == 0)){
    lcd.setCursor(7,0);
    lcd.write(byte(2));
    lcd.setCursor(5,1);
    lcd.print("Paper");
  }
  if ((select == 1)&(blinkState == 1)){
    lcd.setCursor(7,0);
    lcd.write(byte(3));
    lcd.setCursor(5,1);
    lcd.print("Paper");
  }
  if ((select == 2)&(blinkState == 0)){
    lcd.setCursor(12,0);
    lcd.write(byte(4));
    lcd.setCursor(8,1);
    lcd.print("Scissors");
  }
  if ((select == 2)&(blinkState == 1)){
    lcd.setCursor(12,0);
    lcd.write(byte(5));
    lcd.setCursor(8,1);
    lcd.print("Scissors");
  }

  //This controls the blink rate, the 750 is the duration of a blink
  if (millis()-timer>750){
    blinkState = 1-blinkState;
    timer = millis();
  }
}

int displayMessage(String message,int i){
  //Utility function to display line-scrolling text
  String line1 = ""; //Lines of text to display
  String line2 = "";
  String nextWord = ""; //Reads in each word individually
  int flag = 1; //Variable to let the code stop a loop check
  int index = i; //Where we are in the message

  /*
   * This block runs through the message checking word lengths
   * to see if it can add them to line 1 without over running the
   * screen. If it can, it adds them in, otherwise, it moves on to
   * line 2
   */
  while ((i < message.length())&(line1.length()<16)&(flag)){
    //get the next word, by looking for a space
    while ((message.charAt(i) != ' ')&(i < message.length())){
      nextWord = nextWord + message.charAt(i);
      i++;
    }
    //Add in the spaces at the end of the word
    while (message.charAt(i) == ' '){
      i++;
    }
    //Add the word to the line if it's short enough
    if (line1.length()+nextWord.length()<16){
      line1 = line1 + nextWord + " ";
      nextWord = "";
      index = i;
    }
    //Jump out when you can't add more words
    else{
      flag = 0;
    }
  }

  //Reset the flag and add a space to the current word for line 2
  flag = 1;
  nextWord = nextWord + " ";

  //Do the same as above, but for line 2
  while ((i < message.length())&(line2.length()<16)&(flag)){
    while ((message.charAt(i) != ' ')&(i < message.length())){
      nextWord = nextWord + message.charAt(i);
      i++;
    }
    while (message.charAt(i) == ' '){
      i++;
    }
    if (line2.length()+nextWord.length()<16){
      line2 = line2 + nextWord + " ";
      nextWord = "";
    }
    else{
      flag = 0;
    }
  }

  //Draw the 2 lines as parsed
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);

  //Return index so you can come back and scroll the lines
  return index;
}

