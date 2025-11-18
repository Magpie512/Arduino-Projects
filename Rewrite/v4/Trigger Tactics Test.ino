//Declares
//Libraries
#include <LiquidCrystal.h>

// --- LCD Setup ---
//LCD Dimensions
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

//LCD pins
const int RS_PIN = 13; // Register Select pin
const int EN_PIN = 12; // Enable pin
const int D4_PIN = 11; // Data pin 4
const int D5_PIN = 10; // Data pin 5
const int D6_PIN = 9; // Data pin 6
const int D7_PIN = 8; // Data pin 7

// Initialize the LiquidCrystal LCD object
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// --- Component Pins ---
//Buttons
const int button1 = A0; // Connected to A0 (Button 1) -> Will activate Buzzer
const int button0 = A1; // Connected to A1 (Button 0) -> Will activate LED

//Piezo
const int buzzer = 6;

//RGB pins
const int RED_PIN = 3;
const int GREEN_PIN = 5;
const int BLUE_PIN = 4;

// --- Function Declarations ---
void clearScreen();
void displayWelcome();
void setRGBColor(int r, int g, int b);
void activateBuzzer(); // Renamed and simplified
void checkAndTestComponents(); // New function for the interactive logic

// --- Functions Definitions ---

// Clears the LCD screen
void clearScreen(){
    lcd.clear();
}

// Displays the initial welcome message
void displayWelcome() {
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Trigger Tactics");
    lcd.setCursor(0, 1);
    lcd.print("Hardware Test...");
}

// Sets the RGB LED color
void setRGBColor(int r, int g, int b) {
    // Uses analogWrite for PWM pins
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
}

// Plays a short tone on the buzzer
void activateBuzzer() {
    tone(buzzer, 523, 100); // Middle C for 100ms
}

// Reads button presses, updates LCD, and activates components
void checkAndTestComponents() {
    // Read button states (LOW when pressed, using INPUT_PULLUP)
    int state0 = digitalRead(button0);
    int state1 = digitalRead(button1);

    // --- Button 0 Logic (Activate LED) ---
    lcd.setCursor(0, 0);
    lcd.print("B0: ");
    
    // Simple Debounce Check
    if (state0 == LOW) { 
        delay(50); // Debounce delay
        if (digitalRead(button0) == LOW) { // Confirmed press
            lcd.print("LED ON ");
            setRGBColor(0, 255, 0); // Set LED to Green
        } else {
            lcd.print("UP     "); 
            setRGBColor(0, 0, 0); // Turn off LED after bounce check
        }
    } else {
        lcd.print("UP     "); // Display UP
        setRGBColor(0, 0, 0); // Turn LED off when not pressed
    }

    // --- Button 1 Logic (Activate Buzzer) ---
    lcd.setCursor(10, 0);
    lcd.print("B1: ");
    
    // Simple Debounce Check
    if (state1 == LOW) { 
        delay(50); // Debounce delay
        if (digitalRead(button1) == LOW) { // Confirmed press
            lcd.print("BUZZ!");
            activateBuzzer(); 
        } else {
            lcd.print("UP   "); // Was a bounce
        }
    } else {
        lcd.print("UP   "); // Display UP
        noTone(buzzer); // Ensure buzzer is off when released/not pressed
    }
    
    // Update Line 1
    lcd.setCursor(0, 1);
    lcd.print("Press to Test!  "); 
}

// --- Arduino Setup Function ---
void setup() {
    // Start Serial for debugging
    Serial.begin(9600);
    
    // LCD Initialization
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    displayWelcome();
    delay(2000);

    // Pin Mode Setup
    // Buttons: Use INPUT_PULLUP
    pinMode(button0, INPUT_PULLUP);
    pinMode(button1, INPUT_PULLUP);
    
    // RGB Pins: Set as output (PWM pins 3, 5, 4)
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    setRGBColor(0, 0, 0); // Start with LED off
    
    // Buzzer Pin: Set as output
    pinMode(buzzer, OUTPUT);
}

// --- Arduino Loop Function ---
void loop() {
    checkAndTestComponents();
    
    // Short delay to prevent overwhelming the LCD/serial, 
    // although the button logic has delays as well.
    delay(10); 
}
