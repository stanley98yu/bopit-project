/*
   BopIt.ino: Arduino project for an arcade game based off of
              the popular 1990s-2000s toy, Bop It!
   Authors: Jacy Fang, Bryanna Geiger, Tian Low, Stanley Yu
   ENGI 1102 Spring 2018
   Date: 4/1/18
   Reference: https://gist.github.com/c-eastman/5380163
   Credit to Chris Eastman.
*/


// Global constants: pin numbers for LEDs and buttons
const int rled = 2, rbtn = 8;  // Red: 2, 8 BUTTON
const int gled = 3, pot = A5;  // Green: 3, 10 POTENTIOMETER
const int bled = 4, sw = A4;  // Blue: 4, 12 SWITCH
const int startbtn = 9; // Start Btn: 9
const int buzzer = 5;

// ******************** SETUP ********************
// ***********************************************
void setup() {
  //Set leds and buzzer to outputs
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //Set buttons to inputs
  pinMode(rbtn, INPUT);
  pinMode(pot, INPUT);
  pinMode(sw, INPUT);
  pinMode(startbtn, INPUT);
}

// ******************** MAIN LOOP ********************
// ***************************************************
void loop() {
  // Local variables
  int seq[30]; // Sequence of computer moves
  int usr_in = 0; // User input
  int turn = 0; // Player turn
  int clock_time = 500; // Speed of flashing lights
  boolean start = false;

  // Game not in progress, initialize sequence
  if (start == false) {
    usr_in = 0;
    turn = 0;

    // Random sequence of moves per game.
    randomSeed(analogRead(0));
    for (int n = 0; n < 30; n++)
      seq[n] = random(2, 5);
  }

  // Button status
  //int rbtn_st = digitalRead(rbtn);
  //int gbtn_st = digitalRead(pot);
  //int ybtn_st = digitalRead(sw);
  int startbtn_st = digitalRead(startbtn);

  // Push start button to begin.
  if (startbtn_st == HIGH) {
    tone(buzzer, 800);
    delay(100);
    noTone(buzzer);
    start = true;
  }

  // Game in progress
  while (start == true) {
    turn++; // Increment turn

    // Flash the pattern of moves for previous turns.
    for (int i = 0; i < turn; i++) {
      if (seq[i] == 2) // Red
        seqFlash(2, clock_time);
      else if (seq[i] == 3) // Green
        seqFlash(3, clock_time);
      else if (seq[i] == 4) // Blue
        seqFlash(4, clock_time);
    }

    // Player's turn to match pattern
    for (int k = 0; k < turn; k++) {
      while (usr_in == 0) {
        if (digitalRead(rbtn) == HIGH) {
          buttonFlash(2);
          usr_in = 2;
        } else if (analogRead(pot) > 50) {
          buttonFlash(3);
          usr_in = 3;
          delay(10);
        } else if (digitalRead(sw) == 1) {
          buttonFlash(4);
          usr_in = 4;
        } else {
          usr_in = 0;
        }
      }

      if (usr_in != seq[k]) { // Player is incorrect
        start = false;
        for(int i = 0; i < 1; i++) {
            tone(buzzer, 800);
            delay(200);
            noTone(buzzer);
        }
      } else { // Player is correct
        usr_in = 0;
        clock_time -= 15; // Reduce clock time.
        delay(100);
      }
    }

    if(turn > 30) { // Player wins the game.
        for(int i = 0; i < 3; i++) {
            tone(buzzer, 800);
            delay(200);
            noTone(buzzer);
            delay(10);
        }        
    }
  }
}

// ******************** HELPER FUNCTIONS ********************
// **********************************************************

void buttonFlash(int color) { // Flash button colour
  digitalWrite(color, HIGH);
  delay(200);
  digitalWrite(color, LOW);
  delay(200);
}

void seqFlash(int pin, int clock_time) { // Flash in sequence
  digitalWrite(pin, HIGH);
  delay(clock_time);
  digitalWrite(pin, LOW);
  delay(clock_time);
}
