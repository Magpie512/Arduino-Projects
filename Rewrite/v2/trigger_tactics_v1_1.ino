#include <LiquidCrystal.h>

// LCD wiring, a bit messy but it works for now
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 7;
const int backlightPin = 8;

// Keeps track of whether the display was off last time
bool lcdWasOff = true;

// Timestamp used as the starting line for the counter
unsigned long timeMarker = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(backlightPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  // Might add a quick hello message later
}

void loop() {
  // LOW means the switch is pressed which feels slightly unintuitive
  int swState = digitalRead(switchPin);

  if (swState == LOW) {

    // Runs only at the moment the display wakes up
    if (lcdWasOff) {

      // Grab a clean starting time for the timer
      timeMarker = millis();

      digitalWrite(backlightPin, HIGH);
      lcd.display();
      lcd.clear();

      lcdWasOff = false;
    }

    // Basic elapsed time calculation
    unsigned long elapsed = (millis() - timeMarker) / 1000;

    lcd.setCursor(0, 0);
    lcd.print("TIME RUNNING:  ");

    lcd.setCursor(0, 1);
    lcd.print(elapsed);

  } else {

    // Shuts things down once when the switch goes off
    if (!lcdWasOff) {
      digitalWrite(backlightPin, LOW);
      lcd.noDisplay();
      lcdWasOff = true;
    }
  }

  // Leaving this without a delay since the loop runs light enough
}
