#include <LiquidCrystal.h>

// PIN DEFINITIONS (Unchanged)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
const int switchPin = 7;      
const int backlightPin = 8;   
const int buttonOne = 10;     // Button One (Pin 10)
const int buttonTwo = 9;      // Button Two (Pin 9)

// STATE FLAGS AND TIMER VARIABLES (Unchanged)
bool lcdWasOff = true;        
unsigned long startTime = 0;  


void setup() {
  lcd.begin(16, 2);
  pinMode(backlightPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP); 
  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
}


void loop() {
  
  if (digitalRead(switchPin) == LOW) {
    
    // === MASTER SWITCH IS ON ===
    
    // Logic to turn the display back ON when switch is flipped
    if (lcdWasOff == true) {
      startTime = millis(); 
      digitalWrite(backlightPin, HIGH); 
      lcd.display();                   
      lcd.clear();                     
      lcdWasOff = false;
    }
    
    // 1. CHECK BUTTON ONE (PIN 10)
    if (digitalRead(buttonOne) == LOW) {
      // Button One Pressed: Display Message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BUTTON ONE ACTIVE!");
      lcd.setCursor(0, 1);
      lcd.print("Future Function 1");
      delay(1000); // Pause for 1 second to make the message visible
      lcd.clear(); // Clear the screen before resuming timer
    }
    
    // 2. CHECK BUTTON TWO (PIN 9)
    if (digitalRead(buttonTwo) == LOW) {
      // Button Two Pressed: Display Message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BUTTON TWO ACTIVE!");
      lcd.setCursor(0, 1);
      lcd.print("Future Function 2");
      delay(1000); // Pause for 1 second to make the message visible
      lcd.clear(); // Clear the screen before resuming timer
    }
    
    // 3. DISPLAY LOGIC (Timer runs continuously after any message)
    unsigned long elapsedTime = (millis() - startTime) / 1000;
    
    lcd.setCursor(0, 0);
    lcd.print("RUNNING:        ");
    
    lcd.setCursor(0, 1);
    lcd.print(elapsedTime); 
    lcd.print("s   "); 
    
  } else {
    // === MASTER SWITCH IS OFF ===
    if (lcdWasOff == false) {
      digitalWrite(backlightPin, LOW); 
      lcd.noDisplay();                 
      lcdWasOff = true;
    }
  }
}
