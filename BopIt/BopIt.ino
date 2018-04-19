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
const int rled = 2, rbtn = 8;  // Red: 2, 8 
const int gled = 3, pot = A5;  // Green: 3, 10 POTENTIOMETER
const int yled = 4, pr = A4;  // Yellow: 4, 12 PHOTORESISTOR
const int startbtn = 13; // Start Btn: 13
const int buzzer = 5;

// ******************** SETUP ********************
// ***********************************************
void setup() {
  //Set leds and buzzer to outputs
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(yled, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //Set buttons to inputs
  pinMode(rbtn, INPUT);
  pinMode(pot, INPUT);
  pinMode(pr, INPUT);
  pinMode(startbtn, INPUT);

  Serial.begin(9000);
}

// ******************** MAIN LOOP ********************
// ***************************************************
void loop() {
  // Local variables
  int seq[30]; // Sequence of computer moves
  int usr_in = 0; // User input
  int turn = 0; // Player turn
  int clock_time = 1000; // Speed of flashing lights
  boolean start = false;
  Serial.print(analogRead(pot));

  // Game not in progress, initialize sequence
  if (start == false) {
    usr_in = 0;
    turn = 0;
    clock_time = 1000;

    // Random sequence of moves per game.
    randomSeed(analogRead(0));
    for (int n = 0; n < 30; n++)
      seq[n] = random(2, 5);
  }

  // Button status
  int rbtn_st = digitalRead(rbtn);
  int gbtn_st = digitalRead(pot);
  int ybtn_st = digitalRead(pr);
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
      else if (seq[i] == 4) // Yellow
        seqFlash(4, clock_time);
    }

    // Player's turn to match pattern
    for (int k = 0; k < turn; k++) {
      int originalpot = analogRead(pot);
      while (usr_in == 0) {
        if (digitalRead(rbtn) == HIGH) {
          buttonFlash(2);
          usr_in = 2;
        } else if (analogRead(pot) != originalpot) {
          buttonFlash(3);
          usr_in = 3;
        } else if (analogRead(pr) < 200) {
          buttonFlash(4);
          usr_in = 4;
        } else {
          usr_in = 0;
        }
      }

      if (usr_in != seq[k]) { // Player is incorrect
        start = false;
        tone(buzzer, 800);
        delay(3000);
        noTone(buzzer);
        digitalWrite(seq[k], HIGH);
        delay(3000);
        digitalWrite(seq[k], LOW);
      } else { // Player is correct
        usr_in = 0;
        clock_time -= 10; // Reduce clock time.
        delay(500);
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
}

void seqFlash(int pin, int clock_time) { // Flash in sequence
  digitalWrite(pin, HIGH);
  delay(clock_time);
  digitalWrite(pin, LOW);
  delay(clock_time);
}
