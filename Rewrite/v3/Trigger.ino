#include <LiquidCrystal.h>

// LCD Pin
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

// Pin Declares
const int powerSwitch = 7;  // Master ON/OFF/Reset (Slide Switch)
const int cycleBtn = A1;    // Cycle Button (Pushbutton 1)
const int confirmBtn = 9; // Select/Confirm Button (Pushbutton 2)
const int buzzerPin = 13;// Piezo buzzer
int gLed = 8;           // Backlight control

// Custom character byte array
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

// Added volatile flag manage the ON/OFF state
volatile bool isPoweredOn = false;  
volatile unsigned long lastPowerPress = 0;  
  
unsigned long lastButtonTime = 0;
const long buttonDebounceDelay = 200;  

//Debug
int slideState;

// Setup
void setup() {
  // Pin Setup - All buttons/switch use INPUT_PULLUP
  pinMode(cycleBtn, INPUT_PULLUP);  // Pin A1
  pinMode(confirmBtn, INPUT_PULLUP); // Pin 9
  pinMode(powerSwitch, INPUT_PULLUP); // Pin 7 (Slide Switch)
  pinMode(buzzerPin, OUTPUT); // Pin 13
  pinMode(gLed, OUTPUT); // Pin 8 (Backlight)

  // LCD Setup
  lcd.begin(16, 2);
  lcd.clear();
  
  // Hightened brightness
    digitalWrite(gLed, HIGH); // Turn the backlight ON before the intro screen
  
  // Create the custom character (index 0)
  // LCDs can typically store up to 8 custom characters (indices 0 to 7)
  lcd.createChar(0, blastArt);

  // Display intro screen briefly with the custom character
  lcd.setCursor(1, 0); // Start at column 1, row 0 (First line)
  lcd.write((byte)0);  // Display custom character at index 0
  lcd.print("Trigger");
  lcd.write((byte)0);  // Display custom character at index 0

  // Move the cursor to the start of the second line (row 1)
  lcd.setCursor(5, 1); // Start at column 5, row 1 (Second line)
  lcd.write((byte)0);  // Display custom character at index 0
  lcd.print("Tactics");
  lcd.write((byte)0);  // Display custom character at index 0
  
  delay(1500);
  
  // Initial state is OFF
  //digitalWrite(gLed, LOW);  
  //lcd.noDisplay();  
  
  lcd.clear();
}


void loop() {
  // 1. Read the state of the power switch
  // Since INPUT_PULLUP is used, LOW means the switch is activated (ON/closed circuit).
  // HIGH means the switch is inactive/open circuit (OFF).
  int currentSlideState = digitalRead(powerSwitch); 

  // 2. Check if the power state needs to change (Switch has been moved)
  if (currentSlideState == LOW && !isPoweredOn) {
    // Switch is activated (LOW) AND device is currently OFF
    isPoweredOn = true;
    Serial.println("System Power: ON");
    
    // Turn ON the backlight and display
    digitalWrite(gLed, HIGH); 
    lcd.display();
    lcd.clear();
    
    // Display an initial status message when turning ON
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM ONLINE");
    lcd.setCursor(0, 1);
    lcd.print("Ready to Cycle...");

  } else if (currentSlideState == HIGH && isPoweredOn) {
    // Switch is inactive (HIGH) AND device is currently ON
    isPoweredOn = false;
    Serial.println("System Power: OFF");
    
    // Turn OFF the backlight and display
    digitalWrite(gLed, LOW); 
    lcd.clear();
    lcd.noDisplay();
  }
  
  // 3. Main Device Logic (only runs if powered on)
  if (isPoweredOn) {
    // Put your main application logic here, e.g., checking cycleBtn and confirmBtn
    // For now, it just prints a powered-on status to Serial for debug
    Serial.println("Status: Running");
    
    // Example: Read the cycle button state
    // Note: Debouncing and state machine logic for these buttons are not yet implemented 
    // but should go here.
  }
  
  delay(100); // Small delay to prevent fast looping and unnecessary reading
}
