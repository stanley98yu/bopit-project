/*
 * BopIt.ino: Arduino project for an arcade game based off of
 *            the popular 1990s-2000s toy, Bop It!
 * Authors: Jacy Fang, Bryanna Geiger, Tian Low, Stanley Yu
 * ENGI 1102 Spring 2018
 * Date: 4/1/18
 * Reference: https://gist.github.com/c-eastman/5380163
 * Credit to Chris Eastman.
 */


// Global constants: pin numbers for LEDs and buttons
const int rled = 2, rbtn = 8;  // Red: pins 2 & 8
const int gled = 3, gbtn = 10;  // Green: pins 3 & 10
const int yled = 4, ybtn = 12;  // Yellow: pins 4 & 12
const int startbtn = 13; // Start Btn: pin 13

// ******************** SETUP ********************
void setup() {
  //Set leds to outputs
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(yled, OUTPUT);
 
  //Set buttons to inputs
  pinMode(rbtn, INPUT);
  pinMode(gbtn, INPUT);
  pinMode(ybtn, INPUT);

  Serial.begin(9600); // set baud to 9600 / initialize the lcd
  lcdOn(); //turn on lcd
  clearScreen(); //clear the screen
}

// ******************** MAIN LOOP ********************
void loop() {
  // Local variables
  int seq[30]; // Sequence of computer moves
  int usr_in = 0; // User input
  int turn = 0; // Player turn
  int clock_time = 600; // Speed of flashing lights
  boolean start = false;

  // Game not in progress, initialize sequence
  if(start == false) {
    usr_in = 0;
    turn = 0;
    clock_time = 600;
    
    // Random sequence of moves per game.
    randomSeed(analogRead(0));
    for(int n=0; n < 30; n++)
      seq[n] = random(2,5);
  }
  
  // Button status
  int rbtn_st = digitalRead(rbtn);
  int gbtn_st = digitalRead(gbtn);
  int ybtn_st = digitalRead(ybtn);
  int startbtn_st = digitalRead(startbtn);
  
  // Push start button to begin.
  if(startbtn_st == HIGH) {
    Serial.write("     Begin!     "); 
    start = true;
  }
  
  // Game in progress
  while(start == true) {
    turn++; // Increment turn
    
    // Flash the pattern of moves for previous turns.
    for(int i = 0; i < turn; i++) {
      if(seq[i]==2) // Red
        seqFlash(2, clock_time);
      else if(seq[i]==3) // Green
        seqFlash(3, clock_time);
      else if(seq[i]==4) // Yellow
        seqFlash(4, clock_time); 
    }

    // Player's turn to match pattern
    for(int k=0; k < turn; k++) {
      while(usr_in == 0) {
        if(digitalRead(rbtn) == HIGH) {
          buttonFlash(2);
          usr_in = 2;
        } else if(digitalRead(gbtn) == HIGH) {
          buttonFlash(3);
          usr_in = 3;
        } else if(digitalRead(ybtn) == HIGH) {
          buttonFlash(4);
          usr_in = 4;
        } else {
          usr_in = 0;
        }
      }
      
      if(usr_in != seq[k]) { // Player is incorrect
        clearScreen();
        Serial.write("   Game Over!   ");
        digitalWrite(seq[k], HIGH);
        delay(3000);
        digitalWrite(seq[k],LOW);
        clearScreen();
        start=false;
      } else { // Player is correct
         usr_in = 0;
         clock_time -= 10; // Reduce clock time.
         delay(500);
      }
    } 
  }
}

// ******************** HELPER FUNCTIONS ********************
void clearScreen() { //clear the LCD screen
  Serial.write(12); 
}

void lcdOn() { //turn on the LDC
  Serial.write(22); 
}

void buttonFlash(int color) { // Flash button colour
  digitalWrite(color,HIGH);
  delay(200);
  digitalWrite(color,LOW);
}

void seqFlash(int pin, int clock_time) { // Flash in sequence
  digitalWrite(pin,HIGH);
  delay(clock_time); //clock_time gets shorter and shorter as the game progresses, adding a difficult curve
  digitalWrite(pin,LOW);
  delay(clock_time);
}
