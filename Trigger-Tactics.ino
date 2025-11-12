// Mars Briggs 
//2025 - 10 - 16
//Trigger Tactics
#include <LiquidCrystal.h>

/***********************************
 * Public Constants (Musical Notes)*
 ***********************************/
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

/*
// Custom Character for Blast (Looks like a star/explosion)
byte blastArt[] = {
  B00000,
  B00100,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00100
};

byte guardArt[] = {
  B00000,
  B11111,
  B10101,
  B11011,
  B10101,
  B11011,
  B01110,
  B00000
};

byte trickArt[] = {
  B10001,
  B01010,
  B00000,
  B11011,
  B01001,
  B00000,
  B10001,
  B01110
};

byte patchArt[] = {
  B00010,
  B00111,
  B00010,
  B00000,
  B01000,
  B11100,
  B01000,
  B00000
};

byte jamArt[] = {
  B00100,
  B01010,
  B11111,
  B11011,
  B10101,
  B11011,
  B11111,
  B00000
};

*/

// LCD Pin Setup (Adjust if necessary for your wiring)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

//Pin Delares
const int cycleBtn = A0;       // Cycle Button (Moves Selection Forward)
const int confirmBtn = A1;     // Select/Confirm Button
const int powerBtn = 2;        // Third Button, used for interrupt
const int buzzerPin = 10;      // Piezo buzzer

//Leds - added after the fact for pazazz!
int rLed = 13;
int gLed = 8;
int bLed = 9;

// Game States
int playerHP = 5;     //Sets User Hp to 5
int enemyHP = 5;     //As above
int patchUses = 3;  //Limits heals to 3
int blastUses = 2; //Limits direct hit uses to 2

// Added volatile flag manage the ON/OFF state
volatile bool isPoweredOn = false; 
volatile unsigned long lastPowerPress = 0; // Debouncing for jitPrevent

int selectedMove = 0; // 0=Blast, 1=Guard, 2=Trick, 3=Patch, 4=Jam
bool playerStunned = false; 
bool enemyStunned = false;  

//Array of potential Moves to use
String moves[] = {"Blast", "Guard", "Trick", "Patch", "Jam"};
int numMoves = 5;

// Game Phases
//Enum, I choose you
enum GamePhase {
    PHASE_MOVE_SELECT,
    PHASE_MOVE_CONFIRM,
    PHASE_RESOLVE,
    PHASE_STUNNED
};

GamePhase currentPhase = PHASE_MOVE_SELECT;

unsigned long lastButtonTime = 0;
const long buttonDebounceDelay = 200; 

// Function Prototypes
void displayHP();
void buzz(int duration_ms = 300);
void playerDamage(int amount);
void enemyDamage(int amount);
void playerHeal(int amount);
void enemyHeal(int amount);
void resolveTurn(int userMove);
void resolveStunnedTurn(int enemyMove);
void playVictoryTune(); 
void playGameOverTune();
void checkPower(); // Interrupt Service Routine, my beloved
void displayMoveSelection();
bool checkMoveAvailability(int move);

// Setup
void setup() {
  // Pin Setup - Added INPUT_PULLUP for all buttons for robust operation
  pinMode(cycleBtn, INPUT_PULLUP);  
  pinMode(confirmBtn, INPUT_PULLUP); 
  pinMode(powerBtn, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT);

  // LCD Setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Trigger Tactics");
  delay(1500);
  lcd.clear();
  lcd.print("System OFFLINE");

  // Attach Interrupt for Power Button
  // FALLING edge because the button is wired to ground via PULLUP
  attachInterrupt(digitalPinToInterrupt(powerBtn), checkPower, FALLING); 
}

// Attach Interrupt, my beloved
void checkPower() {
  unsigned long now = millis();
  // Simple interrupt debouncing (jits)
  if (now - lastPowerPress < 200) {
    return;
  }
  lastPowerPress = now;
  
  // Toggle the power state
  isPoweredOn = !isPoweredOn;

  if (isPoweredOn) {
    // POWER ON/RESET: Reset all game state variables for a fresh start
    playerHP = 5;
    enemyHP = 5;
    patchUses = 3;
    blastUses = 2;
    playerStunned = false;
    enemyStunned = false;
    currentPhase = PHASE_MOVE_SELECT;
    
    lcd.clear();
    lcd.print("System Online");
    delay(100); 
    displayHP();
  } else {
    // POWER OFF / SHUTDOWN
    lcd.clear();
    lcd.print("System OFFLINE");
  }
}

// Main Loop
void loop() {
  // If not powered on, just wait for the interrupt
  if (!isPoweredOn) {
    delay(100); 
    return;
  }

  // Check for game over (only runs if isPoweredOn is true)
  if (playerHP <= 0 || enemyHP <= 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (enemyHP <= 0 && playerHP > 0) {
      lcd.print("YOU WIN! TACTICIAN!");
      playVictoryTune(); 
    } else if (playerHP <= 0 && enemyHP > 0) {
      lcd.print("YOU LOSE! GAME OVER");
      playGameOverTune(); 
    } else { // Draw/Tie
      lcd.print("Tie! Stalemate.");
      buzz(500); 
    }

    lcd.setCursor(0, 1);
    lcd.print("POWER Btn to Reset");
    
    // System enters an 'idle' state until the Power Button is pressed again (interrupt runs)
    isPoweredOn = false; 
    
    // Loop waits here until the power button is pressed, which calls checkPower() 
    // to reset all state and set isPoweredOn back to true.
    while (!isPoweredOn) {
        delay(100);
    }
    return;
  }

  // Handle Stunned state
  if (playerStunned && currentPhase != PHASE_RESOLVE) {
    currentPhase = PHASE_STUNNED;
  }

  // PHASE MANAGEMENT
  unsigned long currentTime = millis();
  
  switch (currentPhase) {
    case PHASE_STUNNED:
      lcd.clear();
      lcd.print("Stunned! Skip Turn");
      lcd.setCursor(0, 1);
      lcd.print("Enemy attacks!");
      delay(2000);
      resolveStunnedTurn(random(0, numMoves)); 
      playerStunned = false; 
      currentPhase = PHASE_MOVE_SELECT;
      break;
      
    case PHASE_MOVE_SELECT:
      displayMoveSelection();
      
      // Cycle Button (A1) - Cycles through the move options
      if (digitalRead(cycleBtn) == LOW && (currentTime - lastButtonTime > buttonDebounceDelay)) {
        selectedMove = (selectedMove + 1) % numMoves;
        lastButtonTime = currentTime;
        tone(buzzerPin, NOTE_C5, 50);
      }
      
      // Select Button (A2) - Confirms the current selection
      if (digitalRead(confirmBtn) == LOW && (currentTime - lastButtonTime > buttonDebounceDelay)) {
        lastButtonTime = currentTime;
        
        if (!checkMoveAvailability(selectedMove)) {
            lcd.clear();
            lcd.print("Out of Uses!");
            delay(1000);
        } else {
            currentPhase = PHASE_MOVE_CONFIRM;
            lcd.clear();
            lcd.print("Confirm ");
            lcd.print(moves[selectedMove]);
            lcd.setCursor(0, 1);
            lcd.print("SEL:Yes CYC:No"); // SEL (ConfirmBtn/A2) is Yes, CYC (CycleBtn/A1) is No
        }
      }
      break;

    case PHASE_MOVE_CONFIRM:
      
      // Confirm Button (A2) - Finalize move (SEL:Yes)
      if (digitalRead(confirmBtn) == LOW && (currentTime - lastButtonTime > buttonDebounceDelay)) {
        lastButtonTime = currentTime;
        
        // Deduct uses before resolving (if valid move)
        if (selectedMove == 0) blastUses--;
        if (selectedMove == 3) patchUses--;
        
        currentPhase = PHASE_RESOLVE;
        lcd.clear();
        lcd.print("You chose: ");
        lcd.print(moves[selectedMove]);
        delay(1000);
        
        resolveTurn(selectedMove);
        currentPhase = PHASE_MOVE_SELECT;
      }
      
      // Cycle Button (A1) - Return to selection (CYC:No) - acts as the 'back' button
      if (digitalRead(cycleBtn) == LOW && (currentTime - lastButtonTime > buttonDebounceDelay)) {
        lastButtonTime = currentTime;
        currentPhase = PHASE_MOVE_SELECT;
        // The loop will immediately call displayMoveSelection() again
      }
      break;

    case PHASE_RESOLVE:
      // resolveTurn handles the resolution and resets phase to PHASE_MOVE_SELECT
      break;
  }
}




// DISPLAY & HELPER FUNCTIONS

bool checkMoveAvailability(int move) {
    if ((move == 0 && blastUses <= 0) || (move == 3 && patchUses <= 0)) {
        return false;
    }
    return true;
}

void displayMoveSelection() {
  lcd.setCursor(0, 0);
  lcd.print("Choose: ");
  lcd.print(moves[selectedMove]);
  lcd.print("        "); 
  
  lcd.setCursor(0, 1);
  if (selectedMove == 0) {
    lcd.print("Blast Uses: ");
    lcd.print(blastUses);
  } else if (selectedMove == 3) {
    lcd.print("Patch Uses: ");
    lcd.print(patchUses);
  } else {
    lcd.print("HP: Y:");
    lcd.print(playerHP);
    lcd.print(" E:");
    lcd.print(enemyHP);
  }
  lcd.print("    "); 
}

void resolveTurn(int userMove) {
  int enemyMove;
  if (enemyStunned) {
      enemyMove = -1;  // Use -1 to signify a stunned state
      enemyStunned = false; 
      lcd.clear();
      lcd.print("Enemy STUNNED!");
      delay(1000);
  } else {
      enemyMove = random(0, numMoves); 
      lcd.clear();
      lcd.print("Enemy: ");
      lcd.print(moves[enemyMove]);
      delay(1000);
  }

  // Set cursor once before the switch statements
  lcd.setCursor(0, 1);
  
   //Implemented cases to make it easier for me to understand honestly
  // Outer switch for the player's move
  switch (userMove) {
    case 0: // Player chose Blast
      switch (enemyMove) {
        case 0: playerDamage(1); enemyDamage(1); lcd.print("Both Take 1 DMG"); break;
        case 1: lcd.print("Guard Blocked Blast"); break;
        case 2: enemyDamage(2); lcd.print("Blast Hits Trick! -2E"); break;
        case 3: enemyDamage(1); lcd.print("Blast Interrupts Patch! -1E"); break;
        case 4: playerStunned = true; lcd.print("Jam Stuns User!"); break;
        case -1: playerDamage(1); lcd.print("Blast Hits Stunned! -1Y"); break;
      }
      break;

    case 1: // Player chose Guard
      switch (enemyMove) {
        case 0: lcd.print("Guard Blocked Blast"); break;
        case 1: lcd.print("No Effect"); break;
        case 2: playerDamage(2); lcd.print("Trick Bypasses Guard! -2Y"); break;
        case 3: enemyHeal(1); lcd.print("Enemy Heals 1 HP"); break;
        case 4: lcd.print("No Effect"); break;
        case -1: lcd.print("Enemy Stunned, Guard OK"); break;
      }
      break;

    case 2: // Player chose Trick
      switch (enemyMove) {
        case 0: playerDamage(2); lcd.print("Trick Fails! -2Y"); break;
        case 1: enemyDamage(2); lcd.print("Trick Bypasses Guard! -2E"); break;
        case 2: playerDamage(1); enemyDamage(1); lcd.print("Both Take 1 DMG"); break;
        case 3: enemyDamage(1); lcd.print("Trick Interrupts Patch! -1E"); break;
        case 4: playerStunned = true; lcd.print("Jam Stuns User!"); break;
        case -1: enemyDamage(1); lcd.print("Trick Hits Stunned! -1E"); break;
      }
      break;

    case 3: // Player chose Patch
      switch (enemyMove) {
        case 0: playerDamage(1); lcd.print("Blast Interrupts Patch! -1Y"); break;
        case 1: playerHeal(1); lcd.print("User Heals 1 HP"); break;
        case 2: lcd.print("Trick Stops Patch!"); break;
        case 3: playerHeal(1); enemyHeal(1); lcd.print("Both Heal 1 HP"); break;
        case 4: playerStunned = true; lcd.print("Jam Stuns User!"); break;
        case -1: playerHeal(1); lcd.print("Patch Successful! +1Y"); break;
      }
      break;

    case 4: // Player chose Jam
      switch (enemyMove) {
        case 0: enemyStunned = true; lcd.print("Jam Stuns Enemy!"); break;
        case 1: lcd.print("No Effect"); break;
        case 2: playerStunned = true; lcd.print("Jam Stuns User!"); break;
        case 3: enemyStunned = true; lcd.print("Jam Stuns Enemy!"); break;
        case 4: lcd.print("No Effect"); break;
        case -1: lcd.print("No effect, enemy already stunned."); break;
      }
      break;
  }

  delay(1500);
  displayHP();
  delay(1000); 
}

void resolveStunnedTurn(int enemyMove) {
    lcd.clear();
    lcd.print("Enemy Move: ");
    lcd.print(moves[enemyMove]);
    delay(1000);
    
    if (enemyMove == 0) { playerDamage(1); lcd.setCursor(0, 1); lcd.print("Blast hits! -1Y"); } 
    else if (enemyMove == 2) { playerDamage(1); lcd.setCursor(0, 1); lcd.print("Trick hits! -1Y"); } 
    else if (enemyMove == 3) { enemyHeal(1); lcd.setCursor(0, 1); lcd.print("Enemy Heals 1 HP"); } 
    else { lcd.setCursor(0, 1); lcd.print("No effect."); }

    delay(1500);
    displayHP();
    delay(1000);
}

void playerDamage(int amount) {
    playerHP -= amount;
    damage(); 
    playerHP = max(0, playerHP);
}

void enemyDamage(int amount) {
    enemyHP -= amount;
  	damage();
    enemyHP = max(0, enemyHP);
}

void playerHeal(int amount) {
    playerHP += amount;
  	//heals();
    playerHP = min(5, playerHP);
}

//Enemy Heals
void enemyHeal(int amount) {
    enemyHP += amount;
  	//heals();
    enemyHP = min(5, enemyHP);
}


void displayHP() {
  // Update LCD
  lcd.clear();
  lcd.print("You: ");
  lcd.print(playerHP);
  lcd.print(" E:");
  lcd.print(enemyHP);
  
  // Display Uses on line 2
  lcd.setCursor(0, 1);
  lcd.print("B:");
  lcd.print(blastUses);
  lcd.print(" P:");
  lcd.print(patchUses);
}

// TUNE DEFINES
void playVictoryTune() {
  // A simple, ascending triumphant sound
  tone(buzzerPin, NOTE_C5, 100);
  delay(100);
  tone(buzzerPin, NOTE_E5, 100);
  delay(100);
  tone(buzzerPin, NOTE_G5, 100);
  delay(100);
  tone(buzzerPin, NOTE_C6, 400);
  noTone(buzzerPin);
}

void playGameOverTune() {
  // A simple, descending defeated sound
  tone(buzzerPin, NOTE_A4, 200);
  delay(200);
  tone(buzzerPin, NOTE_F4, 200);
  delay(200);
  tone(buzzerPin, NOTE_D4, 400);
  noTone(buzzerPin);
}

//LED indicators
void damage(){ //AI
// Play a descending tone for a "damage taken" sound
  tone(buzzerPin, NOTE_D4, 100);
  delay(100);
  tone(buzzerPin, NOTE_C4, 100);
  delay(100);
  noTone(buzzerPin);
}

void buzz(int duration_ms) { //AI FIX
    // Play a single, short tone for the specified duration
    tone(buzzerPin, NOTE_G4, duration_ms);
    delay(duration_ms + 50); // Add a small delay for a clear tone
    noTone(buzzerPin);
}

/*
void heals(){//AI

// A simple, descending defeated sound
  tone(buzzerPin, NOTE_A4, 200);
  delay(200);
  tone(buzzerPin, NOTE_F4, 200);
  delay(200);
  tone(buzzerPin, NOTE_D4, 400);
  noTone(buzzerPin);

}

void blocked(){//AI

// A simple, descending defeated sound
  tone(buzzerPin, NOTE_A4, 200);
  delay(200);
  tone(buzzerPin, NOTE_F4, 200);
  delay(200);
  tone(buzzerPin, NOTE_D4, 400);
  noTone(buzzerPin);

}

void stunned(){ //AI
  
// A simple, descending defeated sound
  tone(buzzerPin, NOTE_A4, 200);
  delay(200);
  tone(buzzerPin, NOTE_F4, 200);
  delay(200);
  tone(buzzerPin, NOTE_D4, 400);
  noTone(buzzerPin);
  
}
*/
