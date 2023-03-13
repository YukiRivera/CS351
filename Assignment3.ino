/*
    Assignment3: Arduino-based Ping Pong Game Score Tracking Accessory 
    Name: Christopher Nevares, Lucas Rassbach, Yuki Rivera
    Date: 03/12/2023
    Course Code: CS351 - 6633 W-23
    Description: This program keeps track of the scores of each player during a ping-pong game.

*/
#include <LiquidCrystal.h>
#include "scoreboard.h"

// Prepping buttons and global variables
const int pLeftButtonPlus = 3; 
const int pLeftButtonMinus = 4;
const int pRightButtonPlus = 11;
const int pRightButtonMinus = 12;
const int serverButton = 1;

int gameScoreLeft = 0;
int gameScoreRight = 0;
int setScoreLeft = 0;
int setScoreRight = 0;

//instantiate the lcd
// Centralized on board
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

// variable to keep track of the server, PlayerA, PlayerB and the winner.
// The initial server starts from the left
int server = -1;
int playerA = -1;
int playerB = 1;
int winner = 0;

// variable to keep track of score increments
// (every two points the server changes)
int plusCount = 0;
int plusCountIncrement = 1;

// set the LED pins
const int ledPin1 = 2;
const int ledPin2 = 13;
// variable to keep track of how many times the LED should blink
int flashCount = 3;

// prepping LCD for start of game
char* gameScore = setScore(gameScoreLeft, gameScoreRight);
char* setPoint = setScore(setScoreLeft, setScoreRight);


void setup()
{
  //start lcd at 16 x 2
  lcd.begin(16, 2);

  //clear the lcd
  lcd.clear();

 // sets up the pins for LEDs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

}

void loop()
{
  // Lights up the LED on the side of the current server
  turnOnServerLED();
  
//prepping button states
  int buttonStateLeftPlus;  
  int buttonStateLeftMinus;  
  int buttonStateTwoPlus;  
  int buttonStateTwoMinus;

  int serverButtonState;
  serverButtonState = digitalRead(serverButton);

  if((gameScoreLeft + gameScoreRight)%2 == 0) {
    buttonStateLeftPlus = digitalRead(pLeftButtonPlus);  
    buttonStateLeftMinus = digitalRead(pLeftButtonMinus);  
    buttonStateTwoPlus = digitalRead(pRightButtonPlus);  
    buttonStateTwoMinus = digitalRead(pRightButtonMinus);
  } else {
    buttonStateLeftPlus = digitalRead(pRightButtonPlus);  
    buttonStateLeftMinus = digitalRead(pRightButtonMinus);  
    buttonStateTwoPlus = digitalRead(pLeftButtonPlus);  
    buttonStateTwoMinus = digitalRead(pLeftButtonMinus);
  }
  
  // Basic structure of loop is "if button press: increment/decrement score
  //      and update score board".
  /*** when the server button is pressed, it displays who is serving***/
  if(serverButtonState == LOW) {
    displayServer();

  } else if(buttonStateLeftPlus == LOW) {
    setScoreLeft++;
    setPoint = setScore(setScoreLeft, setScoreRight); 
    delay(300);

    plusCount += plusCountIncrement;

  } else if(buttonStateLeftMinus == LOW) {
    setScoreLeft--;
    setPoint = setScore(setScoreLeft, setScoreRight); 
    delay(300);
    
  } else if(buttonStateTwoPlus == LOW) {
    setScoreRight++;
    setPoint = setScore(setScoreLeft, setScoreRight); 
    delay(300);

    plusCount += plusCountIncrement;
    
  } else if (buttonStateTwoMinus == LOW) {
    setScoreRight--;
    setPoint = setScore(setScoreLeft, setScoreRight); 
    delay(300);
    
  } else { /* Do nothing */}

  // Loop to increment game scores.
  if(setScoreLeft >= 11 && setScoreLeft >= setScoreRight + 2) {
    
    winner = -1;
    
    printScore();
    delay(1000);

    // increments the gamescore and resets the set score to zero
    gameScoreLeft++;
    setScoreLeft = 0;
    setScoreRight = 0;

    // blinks LED 3 times for the winner of the current set
    blinkWinnerLED();

    //Adjusts the plusCount increment to 1 for the next game//
    plusCountIncrement = checkCountIncrement();
 
    // Swap the sides of the scoreboard and the players if no side has not won the entire game yet
    if(gameScoreLeft < 3 && gameScoreRight < 3){
      swapScoreboard();
      swapPlayerSide();
    } 

    gameScore = setScore(gameScoreLeft, gameScoreRight);
    setPoint = setScore(setScoreLeft, setScoreRight); 
    
  } else if (setScoreRight >= 11 && setScoreRight >= setScoreLeft + 2) {

    winner = 1;

    printScore();
    delay(1000);

    // increments the gamescore and resets the set score to zero
    gameScoreRight++;
    setScoreLeft = 0;
    setScoreRight = 0;

    // blinks LED 3 times for the winner of the current set
    blinkWinnerLED();

    //Adjusts the plusCount increment to 1 for the next game//
    plusCountIncrement = checkCountIncrement();

    // Swap the sides of the scoreboard and the players if no side has not won the entire game yet
    if(gameScoreLeft < 3 && gameScoreRight < 3){
      swapScoreboard();
      swapPlayerSide();
    } 

    gameScore = setScore(gameScoreLeft, gameScoreRight);
    setPoint = setScore(setScoreLeft, setScoreRight); 
    
  } else{ /* Do nothing */}

  // determines the winner of the entire game//
  if(gameScoreLeft == 3 || gameScoreRight == 3) {
    lcd.clear();
    lcd.setCursor(1, 0);
    flashCount = 15;

    // sets the winner 
    if(gameScoreLeft > gameScoreRight) {
      winner = -1;
    }
    else{
      winner = 1;
    }

    displayWinner();

    // blinks LED 15 times for the winner of the entire game
    blinkWinnerLED(); 
    
    delay(3000);

    //prompting the user to press the reset to start a new game//
    resetPrompt();

    exit(0);

  }

  // prints the current scores on the scoreboard
  printScore();

  // adjusts plusCountIncrement
  plusCountIncrement = checkCountIncrement();

  /*Every two points, the server switches. When the set is deuce with 10-10, 
  the server switches for every point */
  if(plusCount == 2){
      server = server * (-1);
      /*** Resets the count to zero***/ 
      plusCount = 0;
  }

}


//////////Helper Functions/////////
/**
Lights up the LED for the current server.
*/
void turnOnServerLED(){  
  if(server == -1){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
  }
  else if(server == 1){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
  }
}

/**
Displays the current server on the LCD screen 
*/
void displayServer()
{
    lcd.clear();
    lcd.setCursor(3,0);
    if(server == playerA){
      
      lcd.print("Player A is");

    }else{ //server == playerB
      lcd.print("Player B is");
    }
    lcd.setCursor(3,1);
    lcd.print("serving");
    delay(2000);
    lcd.clear(); 
}

/**
Blinks the LED on the side of the winner
*/
void blinkWinnerLED(){
  int x;
  int y;

  if(winner == -1){
    x = ledPin1;
    y = ledPin2;
  }else if(winner == 1) {
    x = ledPin2;
    y = ledPin1;
  }
    while(flashCount > 0){ 
      digitalWrite(x, HIGH);
      digitalWrite(y, LOW); 
      delay(200);              
      digitalWrite(x, LOW);    
      delay(200);  
      flashCount--;
    }
  // resets the flashcount to 3
  flashCount = 3;
}


/** 
Function to adjust the plusCountIncrement.
@return The integer representing the plusCountIncrement.
Returns 2 in the case of deuce, so that the server swaps for 
every button press. Otherwise returns 1 */
int checkCountIncrement(){

    if(setScoreLeft >= 10 && setScoreRight >= 10){
      return 2;
    }else{
      return 1;
    }
}

/**
Swaps the sides of the scoreboard when one set is over
*/
void swapScoreboard() {
    int temp = gameScoreLeft;
    gameScoreLeft = gameScoreRight;
    gameScoreRight = temp; 
  
}

/**
Swap the sides at each player plays the game by multiplying 
each player value by (-1)
*/
void swapPlayerSide(){
  playerA *=(-1);
  playerB *=(-1);
}

/**
Displays the winner on the LCD screen
*/
void displayWinner(){
  if(playerA == winner){
    lcd.print("Player A Wins!");
  }
  else{
    lcd.print("Player B Wins!");
  }
}

/**
Prints the current game score as well as the set points
to the LCD screen
*/
void printScore(){
  //set cursor to column 0, row 0
  lcd.setCursor(0, 0);
  //print the gamescore string over lcd
  lcd.print(gameScore);
  //set cursor to column 0, row 1
  lcd.setCursor(0, 1);
  //print the setpoint string over lcd
  lcd.print(setPoint);

}

/**
Prints the message on the LCD screen to prompts the user to press
the reset button on the Arduino to player a new game
*/
void resetPrompt(){
      
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press reset to");
    lcd.setCursor(0, 1);
    lcd.print("start a new game");
}
