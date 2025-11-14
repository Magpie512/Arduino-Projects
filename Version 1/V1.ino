#include <LiquidCrystal.h>

// Your correct LCD Data Pins: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
const int switchPin = 7; 
const int backlightPin = 8; 

// Flag to track the state change
bool lcdWasOff = true; 

// Variables for managing the restartable timer
unsigned long startTime = 0; // The millis() reading when the display turns ON

void setup() {
  lcd.begin(16, 2);
  pinMode(backlightPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP); 
}

void loop() {
  // Read the switch state. LOW means the switch is closed (ON).
  if (digitalRead(switchPin) == LOW) {
    
    // --- CODE IS RUNNING (SWITCH IS ON) ---
    
    if (lcdWasOff == true) {
      // THIS BLOCK RUNS ONLY ONCE WHEN SWITCH IS FLIPPED ON
      
      // RECORD the current absolute time as the new starting point for the timer
      // We essentially "restart" the timer by setting the offset here.
      startTime = millis(); 
      
      // Display setup
      digitalWrite(backlightPin, HIGH); 
      lcd.display();                   
      lcd.clear();                     
      lcdWasOff = false;
    }
    
    // The main code execution:
    // Calculate elapsed time by subtracting the stored start time from the current time.
    unsigned long elapsedTime = (millis() - startTime) / 1000;
    
    lcd.setCursor(0, 0);
    lcd.print("TIME RUNNING:   ");
    lcd.setCursor(0, 1);
    lcd.print(elapsedTime); 
    
  } else {
    
    // --- CODE IS OFF (SWITCH IS OFF) ---
    
    if (lcdWasOff == false) {
      // THIS BLOCK RUNS ONLY ONCE WHEN SWITCH IS FLIPPED OFF
      
      // Turn OFF Display and set flag
      digitalWrite(backlightPin, LOW); 
      lcd.noDisplay();                 
      lcdWasOff = true;                
    }
  }
}
